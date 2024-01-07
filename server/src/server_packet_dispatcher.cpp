#include "server_packet_dispatcher.h"
#include "server.h"
#include "thread_pool.h"

#include <common/networking/packet.h>

ServerPacketDispatcher::ServerPacketDispatcher(const Server* server)
    : IPacketDispatcher{ server }
{
}

void ServerPacketDispatcher::SendToClient(const Packet& packet, const unsigned int client) const
{
    dynamic_cast<const Server*>(m_handle)->SendToClient(packet, client);
}

void ServerPacketDispatcher::SendToAllClients(const Packet& packet, const unsigned int client) const
{
    dynamic_cast<const Server*>(m_handle)->SendToAllClients(packet, client);
}

void Welcome(const unsigned int client, const std::string& msg)
{
    Packet pckt{ PacketType::Welcome };
    pckt.Write(msg);

    ThreadPool::EnqueuePacketToSend(pckt, false, client);
}

void PlayerConnected(const unsigned int client, const std::string& username)
{
    Packet pckt{ PacketType::PlayerConnected };
    pckt.Write(client);
    pckt.Write(username);

    ThreadPool::EnqueuePacketToSend(pckt, true, client);
}

void PlayerDisconnected(const unsigned int client, const std::string& username)
{
    Packet pckt{ PacketType::PlayerDisconnected };
    pckt.Write(username);

    ThreadPool::EnqueuePacketToSend(pckt, true, client);
}
