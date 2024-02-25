#include "thread_pool.h"

#include <common/utils/logging.h>

#include <algorithm>
#include <ranges>
#include <thread>

#include "server.h"

void ThreadFunction(const UUID id, const ServerPacketHandler& packet_handler,
                    const ServerPacketDispatcher& packet_dispatcher)
{
    SCX_CORE_INFO("Thread {0} has been started!", static_cast<uint64_t>(id));

    while (true)
    {
        const ThreadState state = ThreadPool::GetThreadState(id);

        if (state == ThreadState::WaitingForWork)
        {
            // Don't do any processing when a thread is in a waiting for work state.
            continue;
        }

        if (state == ThreadState::Working)
        {
            // Handle client-related processing here.
            auto packet_info_to_send = ThreadPool::DequeuePacketToSend(id);
            if (packet_info_to_send.has_value())
            {
                Packet& packet = packet_info_to_send.value().packet;
                const unsigned int client_id = packet_info_to_send.value().to_client;

                packet_dispatcher.SendToClient(packet, client_id);
            }

            auto packet_info_to_handle = ThreadPool::DequeuePacketToHandle(id);
            if (packet_info_to_handle.has_value())
            {
                const unsigned int client_id = packet_info_to_handle.value().from_client;
                Packet& packet = packet_info_to_handle.value().packet;

                packet_handler.Handle(client_id, packet, &packet_dispatcher);
            }
        }
    }
}

ThreadPool ThreadPool::s_instance;

ThreadPool::ThreadPool()
{
    m_threads_available = std::min(MAX_THREADS, std::thread::hardware_concurrency());
}

ThreadPool& ThreadPool::Get()
{
    return s_instance;
}


void ThreadPool::Initialise(const ServerPacketHandler& handler, const ServerPacketDispatcher& dispatcher)
{
    for (unsigned int i = 0; i < Get().m_threads_available; i++)
    {
        UUID uuid{};

        Get().m_pool[uuid] = {
            .handle = std::thread{ ThreadFunction, uuid, handler, dispatcher },
            .state = ThreadState::WaitingForWork
        };
    }
}

UUID ThreadPool::AllocateThread()
{
    for (auto& [id, thread] : Get().m_pool)
    {
        if (thread.state == ThreadState::WaitingForWork)
        {
            SCX_CORE_INFO("Allocated thread {0} for processing.", static_cast<uint64_t>(id));

            thread.state = ThreadState::Working;
            return id;
        }
    }

    SCX_CORE_ERROR("No threads available for allocation.");

    return UUID{ 0 };
}

void ThreadPool::TerminateThread(const UUID id)
{
    const auto it = Get().m_pool.find(id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(id));
        return;
    }

    SCX_CORE_INFO("Terminating thread {0} for processing.", static_cast<uint64_t>(id));

    auto& state = it->second.state;
    state = ThreadState::WaitingForWork;
}

void ThreadPool::EnqueuePacketToHandle(const Packet& packet, const unsigned int client_id)
{
    std::unique_lock guard_lock{ Get().m_handle_guard };

    const UUID thread_id = Server::GetClientThreadMap()[client_id];

    const auto it = Get().m_pool.find(thread_id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(thread_id));
        return;
    }

    const PacketInfoFromClient new_packet_info{
        .from_client = client_id,
        .packet = packet
    };

    auto& queue = it->second.handling_queue;
    queue.push(new_packet_info);
}

std::optional<PacketInfoFromClient> ThreadPool::DequeuePacketToHandle(const UUID id)
{
    std::unique_lock guard_lock{ Get().m_handle_guard };

    const auto it = Get().m_pool.find(id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(id));
        return std::nullopt;
    }

    auto& queue = it->second.handling_queue;

    if (queue.empty())
        return std::nullopt;

    PacketInfoFromClient value = queue.front();
    queue.pop();

    return std::make_optional<PacketInfoFromClient>(value);
}

void ThreadPool::EnqueuePacketToSend(const Packet& packet, const unsigned int client_id)
{
    std::unique_lock guard_lock{ Get().m_dispatch_guard };

    const UUID thread_id = Server::GetClientThreadMap()[client_id];

    const auto it = Get().m_pool.find(thread_id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(thread_id));
        return;
    }

    const PacketInfoToClient new_packet_info{
        .to_client = client_id,
        .packet = packet
    };

    auto& queue = it->second.dispatching_queue;
    queue.push(new_packet_info);
}

void ThreadPool::EnqueuePacketToSendToAll(const Packet& packet, const unsigned int except)
{
    for (const auto& client_id : Server::GetClientThreadMap() | std::views::keys)
    {
        if (client_id == except)
            continue;

        EnqueuePacketToSend(packet, client_id);
    }
}

std::optional<PacketInfoToClient> ThreadPool::DequeuePacketToSend(const UUID id)
{
    std::unique_lock guard_lock{ Get().m_dispatch_guard };

    const auto it = Get().m_pool.find(id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(id));
        return std::nullopt;
    }

    auto& queue = it->second.dispatching_queue;

    if (queue.empty())
        return std::nullopt;

    PacketInfoToClient value = queue.front();
    queue.pop();

    return std::make_optional<PacketInfoToClient>(value);
}

ThreadState ThreadPool::GetThreadState(const UUID id)
{
    return Get().m_pool[id].state;
}
