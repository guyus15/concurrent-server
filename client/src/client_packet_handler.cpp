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

ClientPacketHandler::ClientPacketHandler()
    : IPacketHandler{}
{
    // Initialise mapping between packets and their handlers.
    m_handlers =
    {
        { PacketType::Welcome, &Welcome }
    };
}
