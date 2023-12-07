#include "server_packet_dispatcher.h"
#include "server.h"

#include <common/networking/packet.h>

ServerPacketDispatcher::ServerPacketDispatcher(const Server* server)
    : IPacketDispatcher{ server }
{
}

void ServerPacketDispatcher::Welcome(const int to_client, const std::string& msg) const
{
    Packet pckt{ PacketType::Welcome };
    pckt.Write(msg);

    dynamic_cast<const Server*>(m_handle)->SendToClient(pckt, to_client);
}

void ServerPacketDispatcher::NewPlayer(const int to_client, const std::string& msg) const
{
    Packet pckt{ PacketType::NewPlayer };
    pckt.Write(msg);

    dynamic_cast<const Server*>(m_handle)->SendToAllClients(pckt, to_client);
}
