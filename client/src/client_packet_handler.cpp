#include "client_packet_handler.h"
#include "client_packet_dispatcher.h"

#include "client.h"

#include "game/game.h"

#include <common/utils/logging.h>
#include <common/utils/uuid.h>

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

void PlayerHealthUpdate(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    int health;
    packet.Read(health);

    Game::SetLocalPlayerHealth(health);
}

void PlayerDeath(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    unsigned int client_id;
    packet.Read(client_id);

    Game::KillPlayer(client_id);
}

void PlayerRespawn(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
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
        Game::SpawnLocalPlayer(username, player_transform);
    else
        Game::SpawnPlayer(client_id, username, player_transform);
}

void PlayerWeaponRotation(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    unsigned int id;
    packet.Read(id);

    float rotation;
    packet.Read(rotation);

    Game::SetPlayerWeaponRotation(id, rotation);
}

void ProjectileUpdate(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    UUID projectile_id;
    packet.Read(projectile_id);

    glm::vec2 projectile_position;
    packet.Read(projectile_position);

    float projectile_rotation;
    packet.Read(projectile_rotation);

    Game::UpdateProjectile(projectile_id, projectile_position, projectile_rotation);
}

void ProjectileDestroy(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    UUID projectile_id;
    packet.Read(projectile_id);

    Game::DestroyProjectile(projectile_id);
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
        { PacketType::PlayerMovement, &PlayerMovement },
        { PacketType::PlayerHealthUpdate, &PlayerHealthUpdate },
        { PacketType::PlayerDeath, &PlayerDeath },
        { PacketType::PlayerRespawn, &PlayerRespawn },
        { PacketType::PlayerWeaponRotation, &PlayerWeaponRotation },
        { PacketType::ProjectileUpdate, &ProjectileUpdate },
        { PacketType::ProjectileDestroy, &ProjectileDestroy }
    };
}
