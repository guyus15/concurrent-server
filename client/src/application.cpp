#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "asset_manager.h"
#include "window.h"

#include "utils/logging.h"

#include "rendering/shader.h"

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
    Logging::Initialise();

    if (!glfwInit())
        SCX_CORE_CRITICAL("Failed to initialise GLFW.");

    WindowSettings window_settings{};
    window_settings.auto_resolution = true;
    window_settings.width = 600;
    window_settings.height = 400;
    window_settings.window_mode = WindowMode::Windowed;
    m_window = std::make_unique<Window>(window_settings);

    m_window->MakeContextCurrent();

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        SCX_CORE_CRITICAL("Failed to initialise GLAD.\n");
    }
}

void Application::Run() const
{
    const Shader& shader = AssetManager<Shader>::LoadOrRetrieve("E:/Programming/C++/fyp/client/resources/vertex.glsl",
                                                                "E:/Programming/C++/fyp/client/resources/fragment.glsl");
    shader.Use();

    constexpr float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint vbo, vao;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    while (!m_window->ShouldClose())
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        m_window->SwapBuffers();
    }
}

void Application::Dispose()
{
    glfwTerminate();
}
