#include "common/graphics/screen_manager.h"
#include "common/graphics/window.h"

#include "common/events/event_manager.h"

#include "common/utils/logging.h"

void FrameBufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
    const GLFWvidmode* current_mode = glfwGetVideoMode(Window::s_p_callback_instance->GetCurrentMonitor());

    FrameBufferResizeEvent resize_event{};
    resize_event.width = width;
    resize_event.height = height;
    resize_event.refresh_rate = current_mode->refreshRate;

    EventManager::Broadcast(resize_event);
}

Window* Window::s_p_callback_instance = nullptr;

Window::Window(const WindowSettings& settings)
    : m_window_handle{ nullptr }
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

    m_window_handle = glfwCreateWindow(
        mode->width,
        mode->height,
        "Client",
        settings.default_mode == WindowMode::Fullscreen ? m_monitor : nullptr,
        nullptr);

    if (m_window_handle == nullptr)
        SCX_CORE_ERROR("Failed to create GLFW window.");

    s_p_callback_instance = this;

    glfwSetFramebufferSizeCallback(m_window_handle, FrameBufferSizeCallback);

    EventManager::AddListener<WindowResizeEvent>(WindowResizeHandler);
}

Window::~Window()
{
    glfwDestroyWindow(m_window_handle);
}

void Window::MakeContextCurrent() const
{
    glfwMakeContextCurrent(m_window_handle);
}

void Window::SetSize(const int width, const int height) const
{
    glfwSetWindowSize(m_window_handle, width, height);
}

void Window::ToggleFullscreen(const bool fullscreen) const
{
    if (IsFullscreen() == fullscreen)
        return;

    static int width, height;
    static int x_pos, y_pos;

    if (fullscreen)
    {
        // Save the current window size and position.
        // backup window position and window size
        glfwGetWindowSize(m_window_handle, &width, &height);
        glfwGetWindowPos(m_window_handle, &x_pos, &y_pos);

        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

        glfwSetWindowMonitor(m_window_handle, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_window_handle, nullptr, x_pos, y_pos, width, height, 0);
    }
}

void Window::SwapBuffers() const
{
    glfwSwapBuffers(m_window_handle);
}

bool Window::IsFullscreen() const
{
    return glfwGetWindowMonitor(m_window_handle) != nullptr;
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window_handle);
}

GLFWwindow* Window::GetHandle() const
{
    return m_window_handle;
}

GLFWmonitor* Window::GetCurrentMonitor() const
{
    return m_monitor;
}

void Window::WindowResizeHandler(GameEvent& evt)
{
    const auto& window_resize_event = dynamic_cast<WindowResizeEvent&>(evt);

    s_p_callback_instance->SetSize(window_resize_event.width, window_resize_event.height);
    s_p_callback_instance->ToggleFullscreen(window_resize_event.fullscreen);
}
