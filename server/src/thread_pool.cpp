#include "thread_pool.h"

#include <common/utils/logging.h>

#include <algorithm>
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
        }

        if (state == ThreadState::Working)
        {
            // Handle client-related processing here.

            auto packet_info_to_process = ThreadPool::DequeuePacket(id);
            if (packet_info_to_process.has_value())
            {
                const unsigned int from_client = packet_info_to_process.value().from_client;
                Packet& packet = packet_info_to_process.value().packet;

                packet_handler.Handle(from_client, packet, &packet_dispatcher);
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

void ThreadPool::EnqueuePacket(const unsigned int client_id, const Packet& packet)
{
    const UUID thread_id = Server::GetClientThreadMap()[client_id];

    const auto it = Get().m_pool.find(thread_id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(thread_id));
        return;
    }

    const PacketInfo new_packet_info{
        .from_client = client_id,
        .packet = packet
    };

    auto& queue = it->second.processing_queue;
    queue.push(new_packet_info);
}

std::optional<PacketInfo> ThreadPool::DequeuePacket(const UUID id)
{
    const auto it = Get().m_pool.find(id);

    if (it == Get().m_pool.end())
    {
        SCX_CORE_ERROR("Could not find thread with ID {0} in the thread pool.", static_cast<uint64_t>(id));
        return std::nullopt;
    }

    auto& queue = it->second.processing_queue;

    if (queue.empty())
        return std::nullopt;

    PacketInfo value = queue.front();
    queue.pop();

    return std::make_optional<PacketInfo>(value);
}


ThreadState ThreadPool::GetThreadState(const UUID id)
{
    return Get().m_pool[id].state;
}
