#include "thread_pool.h"

#include <common/utils/logging.h>

#include <algorithm>
#include <thread>

void ThreadFunction(const UUID id)
{
    SCX_CORE_INFO("Thread {0} has been started!", static_cast<uint64_t>(id));

    while (true)
    {
        const ThreadState state = ThreadPool::GetThreadState(id);

        if (state == ThreadState::WaitingForWork)
        {
            // Don't do any processing when a thread is in a waiting for work state.
            SCX_CORE_TRACE("Thread {0} is waiting for work.", static_cast<uint64_t>(id));
        }

        if (state == ThreadState::Working)
        {
            // Handle client-related processing here.
            SCX_CORE_TRACE("Thread {0} is working.", static_cast<uint64_t>(id));
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


void ThreadPool::Initialise()
{
    for (unsigned int i = 0; i < Get().m_threads_available; i++)
    {
        UUID uuid{};

        Get().m_pool[uuid] = {
            .handle = std::thread{ ThreadFunction, uuid },
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
        SCX_CORE_ERROR("Could find thread with ID {0} in the thread pool.", static_cast<uint64_t>(id));
        return;
    }

    SCX_CORE_INFO("Terminating thread {0} for processing.", static_cast<uint64_t>(id));

    auto& [_, state] = it->second;
    state = ThreadState::WaitingForWork;
}

ThreadState ThreadPool::GetThreadState(const UUID id)
{
    return Get().m_pool[id].state;
}
