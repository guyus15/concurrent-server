#include "common/graphics/screen_manager.h"

/**
 * \brief Gets the resolution of the current video mode.
 * \param monitor The monitor of which to find the current resolution.
 * \return The resolution of the current video mode.
 */
Resolution GetCurrentModeResolution(GLFWmonitor* monitor)
{
    const GLFWvidmode* current_mode = glfwGetVideoMode(monitor);
    return {
        .width = static_cast<unsigned int>(current_mode->width),
        .height = static_cast<unsigned int>(current_mode->height)
    };
}

ScreenManager ScreenManager::s_instance{};

void ScreenManager::Initialise()
{
    auto [width, height] = GetCurrentModeResolution(glfwGetPrimaryMonitor());

    Get().UpdateResolution(width, height);
}

void ScreenManager::UpdateResolution(const unsigned int width, const unsigned int height)
{
    Get().m_current_resolution = {
        .width = width,
        .height = height
    };
}

Resolution ScreenManager::GetCurrentResolution()
{
    return Get().m_current_resolution;
}

float ScreenManager::GetCurrentAspectRatio()
{
    auto [width, height] = Get().m_current_resolution;

    return static_cast<float>(width) / static_cast<float>(height);
}

ScreenManager& ScreenManager::Get()
{
    return s_instance;
}

std::vector<Resolution> GetAvailableResolutions(GLFWmonitor* monitor)
{
    std::vector<Resolution> available_resolutions{};

    int num_modes;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &num_modes);

    for (int i = 0; i < num_modes; i++)
    {
        Resolution new_resolution{
            .width = static_cast<unsigned int>(modes[i].width),
            .height = static_cast<unsigned int>(modes[i].height)
        };
        available_resolutions.push_back(new_resolution);
    }

    return available_resolutions;
}
