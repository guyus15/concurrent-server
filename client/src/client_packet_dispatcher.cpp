#include "client_packet_dispatcher.h"
#include "client.h"
#include "input.h"

#include "game/game.h"

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

    const bool key_pressed_down_w = Input::GetKeyDown(KeyCode::W);
    const bool key_pressed_a = Input::GetKey(KeyCode::A);
    const bool key_pressed_d = Input::GetKey(KeyCode::D);
    const bool left_mouse_btn_pressed = Input::GetMouse(MouseButton::LeftMouseButton);

    Packet pckt{ PacketType::PlayerInput };
    pckt.Write(key_pressed_down_w);
    pckt.Write(key_pressed_a);
    pckt.Write(key_pressed_d);
    pckt.Write(left_mouse_btn_pressed);

    client_handle->SendToServer(pckt);
}

void ClientPacketDispatcher::PlayerWeaponRotation() const
{
    const auto client_handle = dynamic_cast<const Client*>(m_handle);

    const float rotation = Game::GetLocalPlayerWeaponRotation();

    Packet pckt{ PacketType::PlayerWeaponRotation };
    pckt.Write(rotation);

    client_handle->SendToServer(pckt);
}

void ClientPacketDispatcher::PlayerRespawnRequest() const
{
    const auto client_handle = dynamic_cast<const Client*>(m_handle);

    const unsigned int client_id = Client::GetClientId();

    Packet pckt{ PacketType::PlayerRespawnRequest };
    pckt.Write(client_id);

    client_handle->SendToServer(pckt);
}
