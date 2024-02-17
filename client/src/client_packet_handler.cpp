#include "client_packet_handler.h"
#include "client_packet_dispatcher.h"

#include "client.h"

#include "game/game.h"

#include <common/utils/logging.h>

void Welcome(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    std::string msg;
    packet.Read(msg);

    unsigned int id;
    packet.Read(id);

    SCX_CORE_INFO("(SERVER): {0}", msg);

    Client::SetClientId(id);

    dynamic_cast<const ClientPacketDispatcher*>(dispatcher)->WelcomeReceived();
}

void PlayerConnected(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    unsigned int client_id;
    packet.Read(client_id);

    std::string username;
    packet.Read(username);

    glm::vec2 position;
    packet.Read(position);

    glm::vec2 scale;
    packet.Read(scale);

    Transform player_transform{};
    player_transform.position = position;
    player_transform.scale = scale;
    player_transform.rotation = 0.0f;

    if (client_id == Client::GetClientId())
    {
        SCX_CORE_INFO("You have connected to the server with username {0}.", username);
        Game::SpawnLocalPlayer(username, player_transform);
    }
    else
    {
        SCX_CORE_INFO("{0} has connected to the server ({1}).", username, client_id);
        Game::SpawnPlayer(client_id, username, player_transform);
    }
}

void PlayerDisconnected(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    unsigned int id;
    packet.Read(id);

    std::string username;
    packet.Read(username);

    Game::RemovePlayer(id);

    SCX_CORE_INFO("{0} has disconnected.", username);
}

void PlayerMovement(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    unsigned int id;
    packet.Read(id);

    glm::vec2 position;
    packet.Read(position);

    if (id == Client::GetClientId())
        id = 0;

    Game::SetPlayerPosition(id, position);
}

ClientPacketHandler::ClientPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::Welcome, &Welcome },
        { PacketType::PlayerConnected, &PlayerConnected },
        { PacketType::PlayerDisconnected, &PlayerDisconnected },
        { PacketType::PlayerMovement, &PlayerMovement }
    };
}
