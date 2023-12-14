#include "thread_pool.h"

#include <common/utils/logging.h>

#include <algorithm>
#include <thread>

void ThreadFunction(unsigned int id)
{
    SCX_CORE_INFO("Thread {0} has been started!", id);

    while (ThreadManager::GetThreadActive(id))
    {
        const ThreadState state = ThreadManager::GetThreadState(id);

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

bool ThreadManager::GetThreadActive(const unsigned int id)
{
    // TODO: implement concurrent sychronisation here!
    return Get().m_active_threads[id];
}


ThreadState ThreadManager::GetThreadState(const unsigned int id)
{
    // TODO: implement concurrent sychronisation here!
    return Get().m_thread_states[id];
}

ThreadManager& ThreadManager::Get()
{
    return s_instance;
}

ThreadPool::ThreadPool()
{
    m_threads_available = std::min(MAX_THREADS, std::thread::hardware_concurrency());
}


void ThreadPool::Initialise()
{
    for (unsigned int i = 0; i < m_threads_available; i++)
    {
        m_pool.emplace_back(ThreadFunction, i);
    }
}
