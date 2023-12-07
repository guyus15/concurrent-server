#include "client_packet_dispatcher.h"
#include "client.h"

#include <common/networking/packet.h>

ClientPacketDispatcher::ClientPacketDispatcher(const Client* client)
    : IPacketDispatcher{ client }
{
}

void ClientPacketDispatcher::WelcomeReceived() const
{
    const auto client_handle = dynamic_cast<const Client*>(m_handle);

    Packet pckt{ PacketType::WelcomeReceived };
    pckt.Write(client_handle->m_client_info.username);

    client_handle->SendToServer(pckt);
}
