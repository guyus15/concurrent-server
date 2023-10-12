#include "screen.h"

std::vector<Resolution> GetAvailableResolutions(GLFWmonitor* monitor)
{
    std::vector<Resolution> available_resolutions{};

    int num_modes;
    const GLFWvidmode* modes = glfwGetVideoModes(monitor, &num_modes);

    for (int i = 0; i < num_modes; i++)
    {
        Resolution new_resolution{ modes[i].width, modes[i].height };
        available_resolutions.push_back(new_resolution);
    }

    return available_resolutions;
}

Resolution GetCurrentResolution(GLFWmonitor* monitor)
{
    const GLFWvidmode* current_mode = glfwGetVideoMode(monitor);
    return { current_mode->width, current_mode->height };
}
