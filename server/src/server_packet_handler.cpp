#include "server_packet_handler.h"
#include "server_packet_dispatcher.h"
#include "game.h"
#include "server.h"

#include <common/utils/logging.h>

void WelcomeReceived(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    std::string username;
    packet.Read(username);

    SCX_CORE_INFO("Client has connected with username: {0}", username);

    ClientInfo& client_info = Server::GetClientInfoMap()[client_id];

    client_info.username = username;
    client_info.player.SetId(client_id);

    Game::SpawnPlayer(client_info.player);

    PlayerConnected(client_id, username);
}

void PlayerInput(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    bool inputs[4]{};
    packet.Read(inputs[0]); // W key pressed down
    packet.Read(inputs[1]); // A key pressed
    packet.Read(inputs[2]); // D key pressed
    packet.Read(inputs[3]); // Left mouse button pressed.

    Player& client_player = Server::GetClientInfoMap()[client_id].player;

    client_player.ProcessInput(inputs[0], inputs[1], inputs[2], inputs[3]);
}

void PlayerWeaponRotation(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    float rotation;
    packet.Read(rotation);

    Player& client_player = Server::GetClientInfoMap()[client_id].player;

    client_player.SetWeaponRotation(rotation);

    PlayerWeaponRotation_Dispatch(client_id, client_player);
}

void PlayerRespawnRequest(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    PlayerRespawn(client_id);
}

void ChatMessageReceive(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher)
{
    std::string message;
    packet.Read(message);

    ChatMessageSend(client_id, message);
}

ServerPacketHandler::ServerPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::WelcomeReceived, &WelcomeReceived },
        { PacketType::PlayerInput, &PlayerInput },
        { PacketType::PlayerWeaponRotation, &PlayerWeaponRotation },
        { PacketType::PlayerRespawnRequest, &PlayerRespawnRequest },
        { PacketType::ChatMessageOutbound, &ChatMessageReceive }
    };
}
