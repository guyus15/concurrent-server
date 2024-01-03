#pragma once

#include <common/ui/ui.h>
#include <common/ui/ui_manager.h>

#include <common/events/event_manager.h>

#include <imgui.h>

#include <string>

constexpr int IP_LENGTH = 32;
constexpr int PORT_LENGTH = 8;
constexpr int USERNAME_LENGTH = 32;

void OnConnect(GameEvent& evt);

class ConnectMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        m_title = "Connect";
        m_show = true;

        EventManager::AddListener<OnConnectEvent>(OnConnect);
    }

    void Update(const double delta_time) override
    {
        if (!m_show) return;

        ImGui::Begin(m_title.c_str());

        static char username[USERNAME_LENGTH];
        ImGui::InputTextWithHint("##username", "Username", username, IM_ARRAYSIZE(username));

        static char ip[IP_LENGTH];
        ImGui::InputTextWithHint("##ip-address", "IP Address", ip, IM_ARRAYSIZE(ip));

        static char port[PORT_LENGTH];
        ImGui::InputTextWithHint("##port-no", "Port No", port, IM_ARRAYSIZE(port));

        if (ImGui::Button("Connect"))
        {
            // TODO: Implement input validation to ensure that inputs
            // are of the correct type.

            OnConnectEvent on_connect_event{};
            on_connect_event.username = username;
            on_connect_event.ip = ip;
            on_connect_event.port = std::stoi(port);
            EventManager::Broadcast(on_connect_event);
        }

        ImGui::End();
    
    }

private:
    friend void OnConnect(GameEvent& evt);
};

inline void OnConnect(GameEvent& evt)
{
    const auto& on_connect_event = dynamic_cast<OnConnectEvent&>(evt);

    const std::shared_ptr<UserInterface> ui = UiManager::GetRegisteredUi<ConnectMenu>();
    const auto connect_menu = std::dynamic_pointer_cast<ConnectMenu>(ui);

    connect_menu->m_show = false;
}