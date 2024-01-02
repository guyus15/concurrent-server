#pragma once

#include <GLFW/glfw3.h>

enum class WindowMode
{
    Windowed,
    WindowedFullscreen,
    Fullscreen
};

struct WindowSettings
{
    int width, height;
    bool auto_resolution;
    WindowMode window_mode;
};

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
     * \brief Swaps the buffers associated with this window. 
     */
    void SwapBuffers() const;

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

private:
    GLFWwindow* m_window_handle;
};