#include "client_packet_handler.h"
#include "client_packet_dispatcher.h"

#include <common/utils/logging.h>

void Welcome(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    (void)from;

    std::string msg;
    packet.Read(msg);

    SCX_CORE_INFO("(SERVER): {0}", msg);

    dynamic_cast<const ClientPacketDispatcher*>(dispatcher)->WelcomeReceived();
}

void PlayerConnected(const unsigned int from, Packet& packet, const IPacketDispatcher* dispatcher)
{
    unsigned int client_id;
    packet.Read(client_id);

    std::string username;
    packet.Read(username);

    SCX_CORE_INFO("{0} has connected to the server ({1}).", username, client_id);
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
