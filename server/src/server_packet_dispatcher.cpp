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
    pckt.Write(client);

    ThreadPool::EnqueuePacketToSend(pckt, client);
}

void PlayerConnected(const unsigned int client, const std::string& username)
{
    Packet pckt{ PacketType::PlayerConnected };
    pckt.Write(client);
    pckt.Write(username);

    // Create a new "Player" object in client_info (Server::GetClientInfoMap()) to reference the position.
    constexpr float test_xpos = 0.0f, test_ypos = 0.0f;
    pckt.Write(test_xpos);
    pckt.Write(test_ypos);

    ThreadPool::EnqueuePacketToSendToAll(pckt, 0);
}

void PlayerDisconnected(const unsigned int client, const std::string& username)
{
    Packet pckt{ PacketType::PlayerDisconnected };
    pckt.Write(username);

    ThreadPool::EnqueuePacketToSendToAll(pckt, client);
}
