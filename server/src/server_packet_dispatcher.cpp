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

    static int players_connected = 0;

    // Create a new "Player" object in client_info (Server::GetClientInfoMap()) to reference the position.
    const Player& client_player = Server::GetClientInfoMap()[client].player;
    pckt.Write(client_player.GetPosition());

    ThreadPool::EnqueuePacketToSendToAll(pckt, 0);

    // For every client already connected, send a packet to spawn these on the new client.
    for (const auto& [existing_client_id, existing_client_info] : Server::GetClientInfoMap())
    {
        // Make sure the existing client ID, is not the new client.
        if (existing_client_id == client)
            continue;

        const auto& [existing_username, existing_player] = existing_client_info;

        Packet existing_player_pckt{ PacketType::PlayerConnected };
        existing_player_pckt.Write(existing_client_id);
        existing_player_pckt.Write(existing_username);
        existing_player_pckt.Write(existing_player.GetPosition());

        ThreadPool::EnqueuePacketToSend(existing_player_pckt, client);
    }

    players_connected++;
}

void PlayerDisconnected(const unsigned int client, const std::string& username)
{
    Packet pckt{ PacketType::PlayerDisconnected };
    pckt.Write(client);
    pckt.Write(username);

    ThreadPool::EnqueuePacketToSendToAll(pckt, client);
}

void PlayerMovement(const unsigned int client, const Player& player)
{
    Packet pckt{ PacketType::PlayerMovement };
    pckt.Write(client);
    pckt.Write(player.GetPosition());

    ThreadPool::EnqueuePacketToSendToAll(pckt, 0);
}
