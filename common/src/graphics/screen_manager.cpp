#include "common/graphics/screen_manager.h"

/**
 * \brief Gets the resolution of the current video mode.
 * \param monitor The monitor of which to find the current resolution.
 * \return The resolution of the current video mode.
 */
VideoMode GetCurrentModeResolution(GLFWmonitor* monitor)
{
    const GLFWvidmode* current_mode = glfwGetVideoMode(monitor);
    return {
        .width = current_mode->width,
        .height = current_mode->height,
        .refresh_rate = current_mode->refreshRate
    };
}

ScreenManager ScreenManager::s_instance{};

void ScreenManager::Initialise()
{
    auto [width, height, refresh_rate] = GetCurrentModeResolution(glfwGetPrimaryMonitor());

    UpdateVideoModeResolution(width, height, refresh_rate);
}

void ScreenManager::UpdateVideoModeResolution(const int width, const int height, const int refresh_rate)
{
    Get().m_current_mode = {
        .width = width,
        .height = height,
        .refresh_rate = refresh_rate
    };
}

VideoMode ScreenManager::GetCurrentVideoMode()
{
    return Get().m_current_mode;
}

float ScreenManager::GetCurrentAspectRatio()
{
    auto [width, height, _] = Get().m_current_mode;

    return static_cast<float>(width) / static_cast<float>(height);
}

ScreenManager& ScreenManager::Get()
{
    return s_instance;
}

std::vector<VideoMode> GetAvailableVideoModes(GLFWmonitor* monitor)
{
    std::vector<VideoMode> available_video_modes{};

    int num_modes;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &num_modes);

    for (int i = 0; i < num_modes; i++)
    {
        VideoMode new_resolution{
            .width = modes[i].width,
            .height = modes[i].height,
            .refresh_rate = modes[i].refreshRate
        };
        available_video_modes.push_back(new_resolution);
    }

    return available_video_modes;
}
