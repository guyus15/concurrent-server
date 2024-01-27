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
    unsigned int client_id;
    packet.Read(client_id);

    std::string username;
    packet.Read(username);

    float xpos;
    packet.Read(xpos);

    float ypos;
    packet.Read(ypos);

    const glm::vec2 position{ xpos, ypos };

    if (client_id == Client::GetClientId())
    {
        SCX_CORE_INFO("You have connected to the server with username {0}.", username);
        Game::SpawnLocalPlayer(username, position);
    }
    else
    {
        SCX_CORE_INFO("{0} has connected to the server ({1}).", username, client_id);
        Game::SpawnPlayer(from, username, position);
    }
}

void PlayerDisconnected(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    std::string username;
    packet.Read(username);

    SCX_CORE_INFO("{0} has disconnected.", username);
}

ClientPacketHandler::ClientPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::Welcome, &Welcome },
        { PacketType::PlayerConnected, &PlayerConnected },
        { PacketType::PlayerDisconnected, &PlayerDisconnected }
    };
}
