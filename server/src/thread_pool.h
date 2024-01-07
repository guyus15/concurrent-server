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
     * \brief Adds a packet to a queue of packets awaiting to be processed by a given
     * thread.
     * \param client_id The identifier of the client from which the packet came from.
     * \param packet The packet to be processed.
     */
    static void EnqueuePacketToHandle(const Packet& packet, unsigned int client_id);

    /**
     * \brief Removes and retrieves the next packet from the queue of packets awaiting
     * to be processed, if one exists.
     * \param id The identifier of the thread that will process the next packet.
     * \return An optional packet information value.
     */
    static std::optional<PacketInfoFromClient> DequeuePacketToHandle(UUID id);

    /**
     * \brief Adds a packet to a queue of packets awaiting to be sent by a given thread
     * to its respective client.
     * \param packet The packet to be sent.
     * \param send_to_all Determines whether the packet will be sent to all clients, or only to
     * one specific client.
     * \param client_id The behaviour of the \code client_id\endcode is dependent on the \code send_to_all\endcode
     * parameter. If set to true, the \code client_id\endcode will specify a client to exclude when sending to all
     * clients. If set to false, the \code client_id\endcode will be interpreted as the target for which the packet
     * should be sent to.
     */
    static void EnqueuePacketToSend(const Packet& packet, bool send_to_all, unsigned int client_id = 0);

    /**
     * \brief Removes and retrieves the next packlet from the queue of packets awaiting
     * to be sent, if one exists.
     * \param id The identifier of the thread that will process the next packet.
     * \return An optional packet information value.
     */
    static std::optional<PacketInfoToClient> DequeuePacketToSend(UUID id);

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
        ThreadState state{ ThreadState::WaitingForWork };
        std::queue<PacketInfoToClient> dispatching_queue;
        std::queue<PacketInfoFromClient> handling_queue;
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
