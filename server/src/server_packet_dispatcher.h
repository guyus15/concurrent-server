#pragma once

#include <common/interface/ipacket_dispatcher.h>

#include <string>

class Server;
class Packet;

/**
 * \brief Implementation of \code IPacketDispatcher\endcode for server-side packet dispatching.
 */
class ServerPacketDispatcher final : public IPacketDispatcher
{
public:
    explicit ServerPacketDispatcher(const Server* server);

    ServerPacketDispatcher(const ServerPacketDispatcher&) = default;
    ServerPacketDispatcher& operator=(const ServerPacketDispatcher&) = default;

    ServerPacketDispatcher(ServerPacketDispatcher&&) noexcept = delete;
    ServerPacketDispatcher&& operator=(ServerPacketDispatcher&&) noexcept = delete;

    ~ServerPacketDispatcher() override = default;

    /**
     * \brief Wraps the server's \code SendToClient\endcode function to simplify behaviour.
     * \param packet The packet which will be sent to the client.
     * \param client The client connection to which the packet will be sent.
     */
    void SendToClient(const Packet& packet, unsigned int client) const;

    /**
     * \brief Wraps the server's \code SendToAllClients\endcode function to simplify behaviour.
     * \param packet The packet which will be dispatched to each client.
     * \param client The client connection to exclude from the transmission.
     */
    void SendToAllClients(const Packet& packet, unsigned int client) const;
};

/**
* \brief Sends a welcome message to a client.
* \param client The client to send the message to.
* \param msg The contents of the message.
*/
void Welcome(unsigned int client, const std::string& msg);

/**
 * \brief Sends a new player message to all clients except \code client\endcode
 * to inform them that a new player has connected to the server.
 * \param client The client identifier associated with the new player.
 * \param username The connected player's username.
 */
void PlayerConnected(unsigned int client, const std::string& username);

/**
 * \brief Sends a message to all clients (except \code client\endcode) to indicate
 * that a player has disconnected.
 * \param client The client identifier associated with the player who disconnected.
 * \param username The username of the disconnected client.
 */
void PlayerDisconnected(unsigned int client, const std::string& username);

void PlayerMovement();
