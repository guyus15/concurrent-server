#pragma once

#include <common/interface/ipacket_dispatcher.h>

#include <string>

class Server;

class ServerPacketDispatcher final : public IPacketDispatcher
{
public:
    explicit ServerPacketDispatcher(const Server* server);

    ServerPacketDispatcher(const ServerPacketDispatcher&) = delete;
    ServerPacketDispatcher& operator=(const ServerPacketDispatcher&) = delete;

    ServerPacketDispatcher(ServerPacketDispatcher&&) noexcept = delete;
    ServerPacketDispatcher&& operator=(ServerPacketDispatcher&&) noexcept = delete;

    /**
     * \brief Sends a welcome message to a client.
     * \param to_client The client to send the message to.
     * \param msg The contents of the message.
     */
    void Welcome(int to_client, const std::string& msg) const;

    /**
     * \brief Sends a new player message to all clients except \code to_client\endcode
     * to inform them that a new player has connected to the server.
     * \param to_client The client to exclude when sending the message.
     * \param msg The contents of the message.
     * TODO: Change msg type to be useful to the client receiving it:
     *  - Some client ID
     *  - Player position.
     */
    void NewPlayer(int to_client, const std::string& msg) const;
};
