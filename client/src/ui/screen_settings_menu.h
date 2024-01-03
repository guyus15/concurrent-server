#pragma once

#include <common/events/events.h>

#include <common/graphics/screen_manager.h>

#include <common/ui/ui.h>

#include <imgui.h>

#include <string>

/**
 * \brief Gets the collection of window mode items in a format expected by DearImGui. 
 * \param items A vector of std::string representing the window mode items.
 * \return A single string representing the window mode items.
 */
inline std::string GetWindowModeItems(const std::vector<std::string>& items)
{
    std::stringstream window_mode_items_stream;

    for (const auto& item : items)
    {
        window_mode_items_stream << item << '\0';
    }

    return window_mode_items_stream.str();
}

/**
 * \brief Gets the available resolution items in a format expected by DearImGui.
 * \return A single string representing the available resolution items.
 */
inline std::string GetAvailableResolutionItems()
{
    std::stringstream available_resolution_items_stream;

    for (const auto [width, height] : GetAvailableResolutions(glfwGetPrimaryMonitor()))
    {
        available_resolution_items_stream << width << "x" << height << '\0';
    }

    return available_resolution_items_stream.str();
}

/**
 * \brief Gets a resolution value based on the current available resolution
 * item index.
 * \param current_item_index The current item index.
 * \return A resolution value equivalent to the currently selected item.
 */
inline Resolution GetResolutionFromItem(const int current_item_index)
{
    return GetAvailableResolutions(glfwGetPrimaryMonitor())[current_item_index];
}

class ScreenSettingsMenu final : public UserInterface
{
public:
    void Initialise() override
    {
        m_title = "Screen Settings";
        m_show = true;

        m_window_modes = { "Windowed", "Fullscreen" };
        m_window_mode_items = GetWindowModeItems(m_window_modes);
        m_current_window_mode_item = 0;
        m_available_resolution_items = GetAvailableResolutionItems();
        m_current_available_resolution_item = 0;

        ApplyUpdates();
    }

    void ApplyUpdates() const
    {
        WindowResizeEvent window_resize_event{};

        window_resize_event.fullscreen = m_window_modes[m_current_window_mode_item] == "Fullscreen";

        const auto [new_width, new_height] = GetResolutionFromItem(m_current_available_resolution_item);
        window_resize_event.width = static_cast<int>(new_width);
        window_resize_event.height = static_cast<int>(new_height);

        EventManager::Broadcast(window_resize_event);
    }

    void Update(const double delta_time) override
    {
        if (!m_show) return;

        ImGui::ShowDemoWindow();

        ImGui::Begin(m_title.c_str());

        ImGui::Combo("Window Mode", &m_current_window_mode_item, m_window_mode_items.c_str(),
                     IM_ARRAYSIZE(m_window_mode_items.c_str()));

        // Only show resolution combo box if the current window mode is windowed.
        if (m_window_modes[m_current_window_mode_item] == "Windowed")
        {
            ImGui::Combo("Resolution", &m_current_available_resolution_item, m_available_resolution_items.c_str(),
                IM_ARRAYSIZE(m_available_resolution_items.c_str()));
        }

        if (ImGui::Button("Apply"))
        {
            ApplyUpdates();
        }

        ImGui::End();
    }

private:
    std::vector<std::string> m_window_modes{};
    std::string m_window_mode_items{};
    int m_current_window_mode_item{};
    std::string m_available_resolution_items{};
    int m_current_available_resolution_item{};
};
