#pragma once

#include <common/interface/ipacket_dispatcher.h>

class Client;

/**
 * \brief Implementation of \code IPacketDispatcher\endcode for client-side packet dispatching.
 */
class ClientPacketDispatcher final : public IPacketDispatcher
{
public:
    explicit ClientPacketDispatcher(const Client* client);

    ClientPacketDispatcher(const ClientPacketDispatcher&) = delete;
    ClientPacketDispatcher& operator=(const ClientPacketDispatcher&) = delete;

    ClientPacketDispatcher(ClientPacketDispatcher&&) noexcept = delete;
    ClientPacketDispatcher& operator=(ClientPacketDispatcher&&) noexcept = delete;

    ~ClientPacketDispatcher() override = default;

    /**
     * \brief Sends a acknowledgement to the server that the welcome has been
     * received along with the username that the player would like to adopt.
     */
    void WelcomeReceived() const;

    /**
     * \brief Sends a packet representing the player's current input.
     */
    void PlayerInput() const;

    /**
     * \brief Sends a packet representing the player's current weapon rotation.
     */
    void PlayerWeaponRotation() const;
};
