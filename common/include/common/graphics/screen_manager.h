#pragma once

#include <GLFW/glfw3.h>

#include <utility>
#include <vector>

struct VideoMode
{
    int width, int height, int refresh_rate;
};

/**
 * \brief Provides utilities to manage the application's screen. 
 */
class ScreenManager
{
public:
    static void Initialise();
   
    /**
     * \brief Updates the screen's current video mode with the given width,
     * height and refresh rate values.
     * \param width The new width.
     * \param height The new height.
     * \param refresh_rate The new refresh rate.
     */
    static void UpdateVideoModeResolution(int width, int height, int refresh_rate);

    /**
     * \brief Gets the current video mode of the screen.
     * \return The screen's current video mode.
     */
    [[nodiscard]] static VideoMode GetCurrentVideoMode();

    /**
     * \brief Gets the current aspect ratio of the screen.
     * \return The screen's current aspect ratio.
     */
    [[nodiscard]] static float GetCurrentAspectRatio();

private:
    VideoMode m_current_mode;

    ScreenManager() = default;
    ~ScreenManager() = default;

    static ScreenManager s_instance;
    static ScreenManager& Get();
};

/**
 * \brief Gets all the available video modes for the given monitor.
 * \param monitor The monitor of which to find the available video modes.
 * \return A vector of available video modes.
 */
std::vector<VideoMode> GetAvailableVideoModes(GLFWmonitor* monitor);
