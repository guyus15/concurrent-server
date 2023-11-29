#pragma once

#include <common/interface/ipacket_dispatcher.h>

#include <string>

class Client;

class ClientPacketDispatcher final : public IPacketDispatcher
{
public:
    explicit ClientPacketDispatcher(const Client* client);

    ClientPacketDispatcher(const ClientPacketDispatcher&) = delete;
    ClientPacketDispatcher& operator=(const ClientPacketDispatcher&) = delete;

    ClientPacketDispatcher(ClientPacketDispatcher&&) noexcept = delete;
    ClientPacketDispatcher& operator=(ClientPacketDispatcher&&) noexcept = delete;

    /**
     * \brief Sends a acknowledgement to the server that the welcome has been
     * received along with the username that the player would like to adopt.
     */
    void WelcomeReceived(const std::string& username) const;
};
