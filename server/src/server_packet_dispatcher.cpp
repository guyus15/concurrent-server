#include "server_packet_dispatcher.h"
#include "server.h"

#include <common/networking/packet.h>

ServerPacketDispatcher::ServerPacketDispatcher(const Server* server)
    : IPacketDispatcher{ server }
{
}

void ServerPacketDispatcher::Welcome(const unsigned int client, const std::string& msg) const
{
    Packet pckt{ PacketType::Welcome };
    pckt.Write(msg);

    dynamic_cast<const Server*>(m_handle)->SendToClient(pckt, client);
}

void ServerPacketDispatcher::NewPlayer(const unsigned int client, const std::string& msg) const
{
    Packet pckt{ PacketType::NewPlayer };
    pckt.Write(msg);

    dynamic_cast<const Server*>(m_handle)->SendToAllClients(pckt, client);
}

void ServerPacketDispatcher::PlayerDisconnected(const unsigned int client, const std::string& username) const
{
    Packet pckt{ PacketType::Disconnect };
    pckt.Write(username);

    dynamic_cast<const Server*>(m_handle)->SendToAllClients(pckt, client);
}
