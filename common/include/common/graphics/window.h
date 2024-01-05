#pragma once

#include <common/events/events.h>

#include <GLFW/glfw3.h>

enum class WindowMode
{
    Windowed,
    WindowedFullscreen,
    Fullscreen
};

struct WindowSettings
{
    std::string title;
    WindowMode default_mode;
};

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

/**
 * \brief Provides utilities for window management. 
 */
class Window
{
public:
    Window() = default;
    explicit Window(const WindowSettings& settings);
    ~Window();

    Window(const Window&) = default;
    Window& operator=(const Window&) = default;

    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;

    /**
     * \brief Makes this window the current context. 
     */
    void MakeContextCurrent() const;

    /**
     * \brief Sets the width and height of the window.
     * \param width The new width.
     * \param height The new height.
     */
    void SetSize(int width, int height) const;

    /**
     * \brief Toggles the fullscreen mode of the window.
     * \param fullscreen A value determining whether to enable or disable fullscreen mode.
     */
    void ToggleFullscreen(bool fullscreen) const;

    /**
     * \brief Swaps the buffers associated with this window. 
     */
    void SwapBuffers() const;

    /**
     * \brief Determines whether the window is currently in fullscreen mode.
     * \return A true or false value determining the fullscreen mode of the window.
     */
    [[nodiscard]] bool IsFullscreen() const;

    /**
     * \brief Determines whether this window should be closed.
     * \returns A true or false value indicating if the window should
     * be closed.
     */
    [[nodiscard]] bool ShouldClose() const;

    /**
     * \brief Gets the window's handle.
     * \returns A raw \code GLFWwindow\endcode pointer of the window's
     * handle.
     */
    [[nodiscard]] GLFWwindow* GetHandle() const;

    /**
     * \brief Gets the window's current monitor.
     * \returns A raw \code GLFWmonitor\endcode pointer to the handle of the
     * window's current monitor. 
     */
    [[nodiscard]] GLFWmonitor* GetCurrentMonitor() const;

private:
    GLFWwindow* m_window_handle;
    GLFWmonitor* m_monitor;

    static Window* s_p_callback_instance;

    static void WindowResizeHandler(GameEvent& evt);

    friend void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};