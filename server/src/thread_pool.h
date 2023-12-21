#pragma once

#include <common/utils/uuid.h>

#include <thread>
#include <unordered_map>

constexpr unsigned int MAX_THREADS = 4;

enum class ThreadState
{
    WaitingForWork,
    Working
};

class ThreadPool
{
public:
    ThreadPool(ThreadPool&&) noexcept = delete;
    ThreadPool& operator=(ThreadPool&&) noexcept = delete;

    static void Initialise();
    static bool GetThreadActiveStatus(UUID id);
    static ThreadState GetThreadState(UUID id);

private:
    struct Thread
    {
        std::thread handle;
        bool active_status;
        ThreadState state;
    };

    std::unordered_map<UUID, Thread> m_pool;
    unsigned int m_threads_available;

    ThreadPool();
    ~ThreadPool() = default;

    ThreadPool(const ThreadPool&) = default;
    ThreadPool& operator=(const ThreadPool&) = default;

    static ThreadPool s_instance;
    static ThreadPool& Get();
};
