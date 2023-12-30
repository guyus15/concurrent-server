#pragma once

#include "server_packet_handler.h"
#include "server_packet_dispatcher.h"

#include <common/networking/packet.h>

#include <common/utils/uuid.h>

#include <optional>
#include <queue>
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
    static void Initialise(const ServerPacketHandler& handler, const ServerPacketDispatcher& dispatcher);

    /**
     * \brief Allocates a thread to be used for processing.
     * \return The identifier for the allocated thread.
     */
    static UUID AllocateThread();

    /**
     * \brief Terminates a thread to end its processing.
     */
    static void TerminateThread(UUID id);

    /**
     * \brief Add a packet to a queue of packets awaiting to be processed by a given
     * thread.
     * \param id The identifier of the thread to process the given packet.
     * \param packet The packet to be processed.
     */
    static void EnqueuePacket(UUID id, const Packet& packet);

    /**
     * \brief Removes and retrieves the next packet from the queue of packets awaiting
     * to be processed if one exists
     * \param id The identifier of the thread that will process the next packet.
     * \return An optional packet value.
     */
    static std::optional<Packet> DequeuePacket(UUID id);

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
        ThreadState state;
        std::queue<Packet> processing_queue;
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
