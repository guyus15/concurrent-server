#pragma once

#include <GLFW/glfw3.h>

#include <utility>
#include <vector>

struct Resolution
{
    unsigned int width, height;
};

/**
 * \brief Provides utilities to manage the application's screen. 
 */
class ScreenManager
{
public:
    static void Initialise();
   
    /**
     * \brief Updates the current resolution of the screen with the given width and
     * height values.
     * \param width The new width.
     * \param height The new height.
     */
    static void UpdateResolution(unsigned int width, unsigned int height);

    /**
     * \brief Gets the current resolution of the screen.
     * \return The screen's current resolution.
     */
    [[nodiscard]] static Resolution GetCurrentResolution();

    /**
     * \brief Gets the current aspect ratio of the screen.
     * \return The screen's current aspect ratio.
     */
    [[nodiscard]] static float GetCurrentAspectRatio();

private:
    Resolution m_current_resolution;

    ScreenManager() = default;
    ~ScreenManager() = default;

    static ScreenManager s_instance;
    static ScreenManager& Get();
};

/**
 * \brief Gets all the available resolutions for the given monitor.
 * \param monitor The monitor of which to find the available resolutions.
 * \return A vector of available resolutions.
 */
std::vector<Resolution> GetAvailableResolutions(GLFWmonitor* monitor);
