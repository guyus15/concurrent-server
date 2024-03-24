#pragma once

#include "ui/ui_helper.h"

#include <common/events/event_manager.h>

#include <common/ui/ui.h>
#include <common/ui/ui_manager.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <string>

constexpr int IP_LENGTH = 32;
constexpr int PORT_LENGTH = 8;
constexpr int USERNAME_LENGTH = 32;

void OnConnect(GameEvent& evt);
bool HasErrors(char username[USERNAME_LENGTH], char ip[IP_LENGTH], char port[PORT_LENGTH], std::string& error_string);

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

        const ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always,
                                ImVec2(0.5f, 0.5f));

        static bool has_error = false;
        static std::string error_string;

        ImGui::Begin(m_title.c_str());

        if (has_error)
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), error_string.c_str());

        static char username[USERNAME_LENGTH];
        ImGui::InputTextWithHint("##username", "Username", username, IM_ARRAYSIZE(username));

        static char ip[IP_LENGTH];
        ImGui::InputTextWithHint("##ip-address", "IP Address", ip, IM_ARRAYSIZE(ip));

        static char port[PORT_LENGTH];
        ImGui::InputTextWithHint("##port-no", "Port No", port, IM_ARRAYSIZE(port));

        has_error = HasErrors(username, ip, port, error_string);

        if (has_error)
        {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }

        if (ImGui::Button("Connect"))
        {
            OnConnectEvent on_connect_event{};
            on_connect_event.username = username;
            on_connect_event.ip = ip;
            on_connect_event.port = std::stoi(port);
            EventManager::Broadcast(on_connect_event);
        }

        if (has_error)
        {
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
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

inline bool HasErrors(char username[32], char ip[32], char port[8], std::string& error_string)
{
    if (!HasContent(username))
        goto invalid_username;
    if (!HasContent(ip))
        goto invalid_ip;
    if (!HasContent(port))
        goto invalid_port;

    // Due to the user of exceptions in the standard library function std::stoi() to handle errors
    // when converting from a string to an integer, I am using a try/catch block here. This is an
    // exception in the code where error codes are the preferred approach.
    try
    {
        int portno = std::stoi(port);
    }
    catch ([[maybe_unused]] std::exception const& ex)
    {
        goto invalid_port;
    }

    return false;

invalid_username:
    error_string = "Invalid username.";
    return true;

invalid_ip:
    error_string = "Invalid IP address.";
    return true;

invalid_port:
    error_string = "Invalid port number.";
    return true;
}
