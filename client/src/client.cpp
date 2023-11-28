#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "client.h"

#include "asset_manager.h"

#include "rendering/shader.h"
#include "rendering/texture2d.h"

#include <common/networking/core.h>

#include <common/utils/assertion.h>
#include <common/utils/logging.h>

#include <steam/isteamnetworkingutils.h>

Client* Client::s_p_callback_instance = nullptr;

Client::Client()
    : m_connection{ k_HSteamNetConnection_Invalid },
      m_interface{ nullptr }
{
    Initialise();
}

Client::~Client()
{
    Dispose();
}

void Client::Initialise()
{
    Logging::Initialise("CLIENT");

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

    InitialiseSteamDatagramConnectionSockets();

    m_interface = SteamNetworkingSockets();
}

void Client::Run()
{
    // TODO: Parse port and IP via user interface.
    constexpr uint16_t placeholder_port = 27565;
    const std::string placeholder_ip = "127.0.0.1";

    Connect(placeholder_port, placeholder_ip);

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

        PollIncomingMessages();
        PollConnectionStateChanges();
    }
}

void Client::Dispose()
{
    glfwTerminate();
    ShutdownSteamDatagramConnectionSockets();
}

void Client::Connect(const uint16_t port, const std::string& ip)
{
    SteamNetworkingIPAddr server_address{};

    if (port <= 0)
        SCX_CORE_ERROR("Invalid port {0}.", port);

    if (server_address.IsIPv6AllZeros())
    {
        if (!server_address.ParseString(ip.c_str()))
            SCX_CORE_ERROR("Invalid server address {0}.", ip);

        server_address.m_port = port;
    }

    char display_address[SteamNetworkingIPAddr::k_cchMaxString];
    server_address.ToString(display_address, sizeof(display_address), true);
    SCX_CORE_INFO("Connecting to server at {0}.", display_address);

    SteamNetworkingConfigValue_t options{};
    options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                   reinterpret_cast<void*>(SteamConnectionStatusChangedCallback));

    m_connection = m_interface->ConnectByIPAddress(server_address, 1, &options);
    if (m_connection == k_HSteamNetConnection_Invalid)
        SCX_CORE_ERROR("Failed to create connection.");
}

void Client::PollIncomingMessages() const
{
    while (true)
    {
        ISteamNetworkingMessage* p_incoming_message = nullptr;

        const int num_msgs = m_interface->ReceiveMessagesOnConnection(m_connection, &p_incoming_message, 1);
        if (num_msgs == 0)
            break;
        if (num_msgs < 0)
            SCX_CORE_ERROR("An error occured when checking for messages.");

        // Copy the contents of the message into a '\0'-terminated string.
        std::string message_contents;
        message_contents.assign(static_cast<const char*>(p_incoming_message->m_pData), p_incoming_message->m_cbSize);

        // Release resource handle to the incoming message now it has been copied.

        // Display anything we receive from the server.
        SCX_CORE_INFO("Message received: {0}", message_contents);
    }
}

void Client::PollConnectionStateChanges()
{
    s_p_callback_instance = this;
    m_interface->RunCallbacks();
}


void Client::OnSteamConnectionStatusChangedCallback(const SteamNetConnectionStatusChangedCallback_t* p_info)
{
    // Ensure that a valid connection exists.
    SCX_ASSERT(p_info->m_hConn == m_connection || m_connection == k_HSteamListenSocket_Invalid,
               "A valid connection does not exist.");

    // Determine the state of the client's connection.
    switch (p_info->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_None:
        break;
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            // Display appropriate log message.
            if (p_info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
                SCX_CORE_ERROR("Failed to connect to the remote host.");
            else if (p_info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                SCX_CORE_ERROR("Connection lost to the remote host.");
            else
                SCX_CORE_ERROR("Remote host connection has terminated.");

            // Cleanup the connection.
            m_interface->CloseConnection(p_info->m_hConn, 0, nullptr, false);
            m_connection = k_HSteamNetConnection_Invalid;
            break;
        }
    case k_ESteamNetworkingConnectionState_Connecting:
        // This callback happens when the client starts connecting to the server.
        // This can be ignored.
        break;
    case k_ESteamNetworkingConnectionState_Connected:
        SCX_CORE_INFO("Successfully connected to the server.");
        break;
    default:
        break;
    }
}

void Client::SteamConnectionStatusChangedCallback(const SteamNetConnectionStatusChangedCallback_t* p_info)
{
    s_p_callback_instance->OnSteamConnectionStatusChangedCallback(p_info);
}
