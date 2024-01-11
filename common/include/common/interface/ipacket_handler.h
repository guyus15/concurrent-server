#pragma once

#include "common/networking/packet.h"
#include "common/utils/logging.h"
#include "common/interface/ipacket_dispatcher.h"

#include <unordered_map>

class IPacketHandler
{
public:
    IPacketHandler() = default;

    IPacketHandler(const IPacketHandler&) = default;
    IPacketHandler& operator=(const IPacketHandler&) = default;

    IPacketHandler(IPacketHandler&&) noexcept = default;
    IPacketHandler& operator=(IPacketHandler&&) noexcept = default;

    virtual ~IPacketHandler() = default;

    void Handle(unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher) const
    {
        const auto it = m_handlers.find(packet.GetType());

        if (it != m_handlers.end())
        {
            // Call packet handler associated with this packet type.
            it->second(client_id, packet, dispatcher);
        }
        else
            SCX_CORE_ERROR("No handlers exist for this packet.");
    }

protected:
    std::unordered_map<PacketType, PacketHandler> m_handlers;
};
