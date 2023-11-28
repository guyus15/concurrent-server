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

    void MakeContextCurrent() const;
    void SwapBuffers() const;
    [[nodiscard]] bool ShouldClose() const;
    [[nodiscard]] GLFWwindow* GetHandle() const;

private:
    GLFWwindow* m_window_handle;
};