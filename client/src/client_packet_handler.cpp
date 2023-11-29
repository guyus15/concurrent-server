#include "client_packet_handler.h"
#include "client_packet_dispatcher.h"

#include <common/utils/logging.h>

void Welcome(Packet& packet, const IPacketDispatcher* dispatcher)
{
    std::string msg;
    packet.Read(msg);

    SCX_CORE_INFO("(SERVER): {0}", msg);

    dynamic_cast<const ClientPacketDispatcher*>(dispatcher)->WelcomeReceived("testUsername2");
}

ClientPacketHandler::ClientPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::Welcome, &Welcome }
    };
}
