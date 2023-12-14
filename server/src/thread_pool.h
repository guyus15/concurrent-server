#pragma once

#include <thread>
#include <vector>

constexpr unsigned int MAX_THREADS = 4;

enum class ThreadState
{
    WaitingForWork,
    Working
};

class ThreadManager
{
public:
    ThreadManager(ThreadManager&&) noexcept = delete;
    ThreadManager& operator=(ThreadManager&&) noexcept = delete;

    static bool GetThreadActive(unsigned int id);

    static ThreadState GetThreadState(unsigned int id);

private:
    std::vector<bool> m_active_threads;
    std::vector<ThreadState> m_thread_states;

    ThreadManager() = default;
    ~ThreadManager() = default;

    ThreadManager(const ThreadManager&) = default;
    ThreadManager& operator=(const ThreadManager&) = default;

    static ThreadManager s_instance;
    static ThreadManager& Get();
};

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool() = default;

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) noexcept = delete;
    ThreadPool& operator=(ThreadPool&&) noexcept = delete;

    void Initialise();

private:
    unsigned int m_threads_available;
    std::vector<std::thread> m_pool;
};