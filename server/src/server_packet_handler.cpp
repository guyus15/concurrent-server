#include "server_packet_handler.h"
#include "server_packet_dispatcher.h"

#include <common/utils/logging.h>

#include "server.h"

void WelcomeReceived(const unsigned int client_id, Packet& packet, const IPacketDispatcher* dispatcher = nullptr)
{
    std::string username;
    packet.Read(username);

    SCX_CORE_INFO("Client has received welcome and connected with username: {0}", username);

    Server::GetClientInfoMap()[client_id].username = username;
}

ServerPacketHandler::ServerPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::WelcomeReceived, &WelcomeReceived }
    };
}
