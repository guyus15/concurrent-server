#include "common/graphics/screen_manager.h"
#include "common/graphics/window.h"

#include <common/events/event_manager.h>

#include "common/utils/logging.h"

void FrameBufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    FrameBufferResizeEvent resize_event{};
    resize_event.width = width;
    resize_event.height = height;

    EventManager::Broadcast(resize_event);
}

Window::Window(const WindowSettings& settings)
    : m_window_handle{ nullptr }
{
    int window_width = settings.width;
    int window_height = settings.height;

    if (settings.auto_resolution)
    {
        const auto [width, height] = ScreenManager::GetCurrentResolution();
        window_width = width;
        window_height = height;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window_handle = glfwCreateWindow(
        window_width,
        window_height,
        "Client",
        settings.window_mode == WindowMode::Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr);

    if (m_window_handle == nullptr)
        SCX_CORE_ERROR("Failed to create GLFW window.");

    glfwSetFramebufferSizeCallback(m_window_handle, FrameBufferSizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(m_window_handle);
}

void Window::MakeContextCurrent() const
{
    glfwMakeContextCurrent(m_window_handle);
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_window_handle);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window_handle);
}

GLFWwindow* Window::GetHandle() const
{
    return m_window_handle;
}
