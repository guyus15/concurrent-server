#pragma once

#include <common/events/event_manager.h>

#include <common/ui/ui.h>
#include <common/ui/ui_manager.h>

#include <imgui.h>

void OnLocalPlayerDeath(GameEvent& evt);

class RespawnMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        m_title = "Respawn";
        m_show = false;

        EventManager::AddListener<OnLocalPlayerDeathEvent>(OnLocalPlayerDeath);
    }

    void Update(const double delta_time) override
    {
        if (!m_show) return;

        ImGui::Begin(m_title.c_str());

        if (ImGui::Button("Respawn"))
        {
            
        }

        ImGui::End();
    }

private:
    friend void OnLocalPlayerDeath(GameEvent& evt);
};

inline void OnLocalPlayerDeath(GameEvent& evt)
{
    const auto& on_local_player_death_event = dynamic_cast<OnLocalPlayerDeathEvent&>(evt);

    const std::shared_ptr<UserInterface> ui = UiManager::GetRegisteredUi<RespawnMenu>();
    const auto respawn_menu = std::dynamic_pointer_cast<RespawnMenu>(ui);

    respawn_menu->m_show = true;
}