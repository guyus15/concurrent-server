#pragma once

#include <common/interface/ipacket_handler.h>

/**
 * \brief Implementation of \code IPacketHandler\endcode for client-side packet handling.
 */
class ClientPacketHandler final : public IPacketHandler
{
public:
    ClientPacketHandler();
    ~ClientPacketHandler() override = default;

    ClientPacketHandler(const ClientPacketHandler&) = default;
    ClientPacketHandler& operator=(const ClientPacketHandler&) = default;

    ClientPacketHandler(ClientPacketHandler&&) noexcept = default;
    ClientPacketHandler& operator=(ClientPacketHandler&&) noexcept = default;
};
