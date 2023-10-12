#include "application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

#include <iostream>

Application::Application()
{
    Initialise();
}

Application::~Application()
{
    Dispose();
}

void Application::Initialise()
{
    if (!glfwInit())
        std::cerr << "Error: Failed to initialise GLFW.\n";

    WindowSettings window_settings{};
    window_settings.auto_resolution = true;
    window_settings.width = 600;
    window_settings.height = 400;
    window_settings.window_mode = WindowMode::Windowed;
    m_window = std::make_unique<Window>(window_settings);

    m_window->MakeContextCurrent();

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Error: Failed to initialise GLAD.\n";
    }
}

void Application::Run() const
{
    while (!m_window->ShouldClose())
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
        m_window->SwapBuffers();
    }
}

void Application::Dispose()
{
    glfwTerminate();
}