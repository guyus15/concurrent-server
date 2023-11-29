#pragma once

#include <common/interface/ipacket_handler.h>

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
