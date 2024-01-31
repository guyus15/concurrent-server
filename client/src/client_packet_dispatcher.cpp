#include "client_packet_dispatcher.h"
#include "client.h"
#include "input.h"

#include <common/networking/packet.h>

ClientPacketDispatcher::ClientPacketDispatcher(const Client* client)
    : IPacketDispatcher{ client }
{
}

void ClientPacketDispatcher::WelcomeReceived() const
{
    const auto client_handle = dynamic_cast<const Client*>(m_handle);

    Packet pckt{ PacketType::WelcomeReceived };
    pckt.Write(client_handle->m_client_info.username);

    client_handle->SendToServer(pckt);
}

void ClientPacketDispatcher::PlayerInput() const
{
    const auto client_handle = dynamic_cast<const Client*>(m_handle);

    Packet pckt{ PacketType::PlayerInput };
    pckt.Write(Input::GetKey(KeyCode::W));
    pckt.Write(Input::GetKey(KeyCode::A));
    pckt.Write(Input::GetKey(KeyCode::S));
    pckt.Write(Input::GetKey(KeyCode::D));

    client_handle->SendToServer(pckt);
}
