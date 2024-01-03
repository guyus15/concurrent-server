#pragma once

#include <common/graphics/screen_manager.h>

#include <common/ui/ui.h>

#include <imgui.h>

#include <string>

inline std::string GetAvailableResolutionItems()
{
    std::stringstream available_resolution_items_stream;

    for (const auto [width, height] : GetAvailableResolutions(glfwGetPrimaryMonitor()))
    {
        available_resolution_items_stream << width << "x" << height << '\0';
    }

    return available_resolution_items_stream.str();
}

class ScreenSettingsMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        m_title = "Screen Settings";
        m_show = true;
        m_available_resolution_items = GetAvailableResolutionItems();
        m_current_available_resolution_item = 0;
    }

    void Update(const double delta_time) override
    {
        if (!m_show) return;

        ImGui::ShowDemoWindow();

        ImGui::Begin(m_title.c_str());

        const char* window_mode_items[] = { "Windowed", "Fullscreen" };
        static int current_window_mode_item = 0;

        ImGui::Combo("Window Mode", &current_window_mode_item, window_mode_items, IM_ARRAYSIZE(window_mode_items));

        // Only render the available resolutions if the current window mode is windowed.
        if (strcmp(window_mode_items[current_window_mode_item], "Windowed") == 0)
        {
            ImGui::Combo("Resolution", &m_current_available_resolution_item, m_available_resolution_items.c_str(),
                         IM_ARRAYSIZE(m_available_resolution_items.c_str()));
        }

        ImGui::End();
    }

private:
    std::string m_available_resolution_items{};
    int m_current_available_resolution_item{};
};
