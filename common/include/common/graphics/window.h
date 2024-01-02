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

struct Dimensions
{
    int width, height;
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
    void UpdateDimensions(int width, int height);

    [[nodiscard]] bool ShouldClose() const;
    [[nodiscard]] Dimensions GetDimensions() const;
    [[nodiscard]] GLFWwindow* GetHandle() const;

private:
    GLFWwindow* m_window_handle;
    Dimensions m_dimensions;
};