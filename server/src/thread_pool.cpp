#include "thread_pool.h"

#include <common/utils/logging.h>

#include <algorithm>
#include <thread>

void ThreadFunction(const UUID id)
{
    SCX_CORE_INFO("Thread {0} has been started!", static_cast<uint64_t>(id));

    while (ThreadPool::GetThreadActiveStatus(id))
    {
        const ThreadState state = ThreadPool::GetThreadState(id);

        if (state == ThreadState::WaitingForWork)
        {
            // Don't do any processing when a thread is in a waiting for work state.
        }

        if (state == ThreadState::Working)
        {
            // Handle client-related processing here.
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
        Get().m_pool[uuid] = { std::thread{ ThreadFunction, uuid }, true, ThreadState::WaitingForWork };
    }
}

bool ThreadPool::GetThreadActiveStatus(const UUID id)
{
    return Get().m_pool[id].active_status;
}

ThreadState ThreadPool::GetThreadState(const UUID id)
{
    return Get().m_pool[id].state;
}
