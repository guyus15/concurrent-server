#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"

#include "asset_manager.h"
#include "window.h"

#include "utils/logging.h"

#include "rendering/shader.h"
#include "rendering/texture2d.h"

#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>

#pragma comment(lib,"WS2_32")

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

    WSADATA wsa_data;

    int i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0)
    {
        SCX_CORE_ERROR("Failed to initialise WinSock.");
    }

    addrinfo *result = nullptr, *ptr = nullptr, hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    const char* address = "127.0.0.1";
    i_result = getaddrinfo(address, "27565", &hints, &result);
    if (i_result != 0)
    {
        SCX_CORE_ERROR("GetAddressInfo() failed: {0}", i_result);
        WSACleanup();
    }

    ptr = result;
    SOCKET connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (connect_socket == INVALID_SOCKET)
    {
        SCX_CORE_ERROR("Socket() failed: {0}", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
    }

    // Connect to the server.
    i_result = connect(connect_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
    if (i_result == SOCKET_ERROR)
    {
        closesocket(connect_socket);
        connect_socket = INVALID_SOCKET;
    }

    // TODO: This should try the next address returned by getaddrinfo if the connect call failed.
    // For this example we just free the resources returned by getaddrinfo and display an
    // error message.

    freeaddrinfo(result);

    if (connect_socket == INVALID_SOCKET)
    {
        SCX_CORE_ERROR("Unable to connect to server.");
        WSACleanup();
    }
    else
    {
        SCX_CORE_INFO("Successfully connected to the server.");
    }

    int recv_buf_len = 512;
    char recv_buf[512];
    const char* send_buf = "this is a test message";

    i_result = send(connect_socket, send_buf, static_cast<int>(strlen(send_buf)), 0);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to send: {0}", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
    }

    SCX_CORE_INFO("Bytes sent: {0}", i_result);

    i_result = shutdown(connect_socket, SD_SEND);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to shutdown: {0}", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
    }

    // Do data until the server closes the connection.
    do
    {
        i_result = recv(connect_socket, recv_buf, recv_buf_len, 0);
        if (i_result > 0)
            SCX_CORE_INFO("Bytes received: {0}", i_result);
        else if (i_result == 0)
            SCX_CORE_INFO("Connection closed.");
        else
            SCX_CORE_INFO("Failed to recv: {0}", WSAGetLastError());
    } while (i_result > 0);

    // Disconnect
    i_result = shutdown(connect_socket, SD_SEND);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to shutdown: {0}", WSAGetLastError());
        closesocket(connect_socket);
        WSACleanup();
    }

    closesocket(connect_socket);
    WSACleanup();
}

void Application::Run() const
{
    const Shader& shader = AssetManager<Shader>::LoadOrRetrieve(
        "resources/shaders/vertex.glsl",
        "resources/shaders/fragment.glsl");
    shader.Use();

    const Texture2d texture = AssetManager<Texture2d>::LoadOrRetrieve(
        "resources/textures/test.png");

    constexpr float vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top left
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f // Bottom right
    };

    constexpr GLuint indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GLuint vbo, vao, ebo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // Vertex colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Vertex texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    while (!m_window->ShouldClose())
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.SetBool("use_texture", true);
        texture.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwPollEvents();
        m_window->SwapBuffers();
    }
}

void Application::Dispose()
{
    glfwTerminate();
}
