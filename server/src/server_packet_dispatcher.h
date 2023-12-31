#pragma once

#include <common/interface/ipacket_dispatcher.h>

#include <string>

class Server;

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
     * \brief Sends a welcome message to a client.
     * \param client The client to send the message to.
     * \param msg The contents of the message.
     */
    void Welcome(unsigned int client, const std::string& msg) const;

    /**
     * \brief Sends a new player message to all clients except \code client\endcode
     * to inform them that a new player has connected to the server.
     * \param client The client identifier associated with the new player.
     * \param msg The contents of the message.
     * TODO: Change msg type to be useful to the client receiving it:
     *  - Some client ID
     *  - Player position.
     */
    void NewPlayer(unsigned int client, const std::string& msg) const;

    /**
     * \brief Sends a message to all clients (except \code client\endcode) to indicate
     * that a player has disconnected.
     * \param client The client identifier associated with the player who disconnected.
     * \param username The username of the disconnected client.
     */
    void PlayerDisconnected(unsigned int client, const std::string& username) const;
};
