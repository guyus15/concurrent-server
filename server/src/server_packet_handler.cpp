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
    bool inputs[3];
    packet.Read(inputs[0]); // W key pressed down
    packet.Read(inputs[1]); // A key pressed
    packet.Read(inputs[2]); // D key pressed

    Player& client_player = Server::GetClientInfoMap()[client_id].player;

    client_player.ProcessInput(inputs[0], inputs[1], inputs[2]);
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
