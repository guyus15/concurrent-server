#include "client_packet_dispatcher.h"
#include "client.h"

#include <common/networking/packet.h>

ClientPacketDispatcher::ClientPacketDispatcher(const Client* client)
    : IPacketDispatcher{ client }
{
}

void ClientPacketDispatcher::WelcomeReceived(const std::string& username) const
{
    Packet pckt{ PacketType::WelcomeReceived };
    pckt.Write(username);

    dynamic_cast<const Client*>(m_handle)->SendToServer(pckt);
}
