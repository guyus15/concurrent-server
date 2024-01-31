#include "server_packet_handler.h"
#include "server_packet_dispatcher.h"

#include <common/utils/logging.h>

#include "server.h"

void WelcomeReceived(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    std::string username;
    packet.Read(username);

    SCX_CORE_INFO("Client has connected with username: {0}", username);

    Server::GetClientInfoMap()[client_id].username = username;

    PlayerConnected(client_id, username);
}

void PlayerInput(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    bool inputs[4];
    packet.Read(inputs[0]); // W key pressed
    packet.Read(inputs[1]); // A key pressed
    packet.Read(inputs[2]); // S key pressed
    packet.Read(inputs[3]); // D key pressed

    Player& client_player = Server::GetClientInfoMap()[client_id].player;
    glm::vec2 current_pos = client_player.GetPosition();

    if (inputs[0])
        client_player.SetPosition({ current_pos.x, current_pos.y + 0.01f });
    if (inputs[1])
        client_player.SetPosition({ current_pos.x - 0.01f, current_pos.y });
    if (inputs[2])
        client_player.SetPosition({ current_pos.x, current_pos.y - 0.01f });
    if (inputs[3])
        client_player.SetPosition({ current_pos.x + 0.01f, current_pos.y });
}

ServerPacketHandler::ServerPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::WelcomeReceived, &WelcomeReceived },
        { PacketType::PlayerInput, &PlayerInput }
    };
}
