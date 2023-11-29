#pragma once

#include <common/interface/ipacket_handler.h>

class ServerPacketHandler final : public IPacketHandler
{
public:
    ServerPacketHandler();
    ~ServerPacketHandler() override = default;

    ServerPacketHandler(const ServerPacketHandler&) = default;
    ServerPacketHandler& operator=(const ServerPacketHandler&) = default;

    ServerPacketHandler(ServerPacketHandler&&) noexcept = default;
    ServerPacketHandler& operator=(ServerPacketHandler&&) noexcept = default;
};
