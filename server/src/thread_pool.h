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

/**
 * \brief A data structure implemented as a singleton pattern to manage a collection
 * of reusable threads.
 */
class ThreadPool
{
public:
    ThreadPool(ThreadPool&&) noexcept = delete;
    ThreadPool& operator=(ThreadPool&&) noexcept = delete;

    /**
     * \brief Initialises each of the threads to be managed by the thread pool.
     * It specifies the initial state and active status of each created thread.
     */
    static void Initialise();

    /**
     * \brief Gets the active status of the thread with the given identifier.
     * \param id The identifier of the thread.
     * \return A true or false value indicating whether the thread is active.
     */
    static bool GetThreadActiveStatus(UUID id);

    /**
     * \brief Gets the state of the thread with the given identifier.
     * \param id The identifier of the thread.
     * \return The current state of the thread.
     */
    static ThreadState GetThreadState(UUID id);

private:
    /**
     * \brief Represents a thread as an object with a state and activity status.
     * Typically, C++ threads do not have associated members, but this allows us
     * to provide them.
     */
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
