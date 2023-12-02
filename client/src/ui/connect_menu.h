#pragma once

#include <common/ui/ui.h>
#include <common/ui/ui_manager.h>

#include <common/events/event_manager.h>

#include <imgui.h>

#include <string>

const std::string TITLE = "Connect";
constexpr int IP_LENGTH = 32;
constexpr int PORT_LENGTH = 8;
constexpr int USERNAME_LENGTH = 32;

void OnConnect(GameEvent& evt);

class ConnectMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        m_title = TITLE;
        m_show = true;

        EventManager::AddListener<OnConnectEvent>(OnConnect);
    }

    void Update(const double delta_time) override
    {
        if (m_show)
        {
            //ImGui::ShowDemoWindow();

            ImGui::Begin(m_title.c_str());

            static char ip[IP_LENGTH];
            ImGui::InputTextWithHint("IP Address", "Type here", ip, IM_ARRAYSIZE(ip));

            static char port[PORT_LENGTH];
            ImGui::InputTextWithHint("Port No", "Type here", port, IM_ARRAYSIZE(port));

            static char username[USERNAME_LENGTH];
            ImGui::InputTextWithHint("Username", "Type here", username, IM_ARRAYSIZE(username));

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