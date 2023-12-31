#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "client.h"

#include "asset_manager.h"

#include "rendering/shader.h"
#include "rendering/texture2d.h"

#include <common/networking/core.h>
#include <common/networking/packet.h>

#include <common/ui/ui_manager.h>

#include <common/utils/assertion.h>
#include <common/utils/logging.h>

#include <steam/isteamnetworkingutils.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

Client* Client::s_p_callback_instance = nullptr;

Client::Client()
    : m_dispatcher{ this },
      m_connection{ k_HSteamNetConnection_Invalid },
      m_interface{ nullptr },
      m_last_time{}
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

    const std::string glsl_version = "#version 430";

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

    // Initialise the UI manager.
    UiManager::Initialise();

    // Setup SteamGameNetworkingSockets.
    InitialiseSteamDatagramConnectionSockets();
    m_interface = SteamNetworkingSockets();

    // Setup Dear ImGui context.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(m_window->GetHandle(), true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    EventManager::AddListener<OnConnectEvent>(OnConnectHandler);

    s_p_callback_instance = this;

    SCX_CORE_INFO("Application initialised.");
}

void Client::Run()
{
    // TODO: Parse port and IP via user interface.
    constexpr uint16_t placeholder_port = 27565;
    const std::string placeholder_ip = "127.0.0.1";

    //Connect(placeholder_port, placeholder_ip);

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
        const double current_time = glfwGetTime();
        const double delta_time = current_time - m_last_time;
        m_last_time = current_time;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.SetBool("use_texture", true);
        texture.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // UI begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        UiManager::Update(delta_time);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // UI end

        glfwPollEvents();
        m_window->SwapBuffers();

        if (m_connection != k_HSteamListenSocket_Invalid)
        {
            PollIncomingMessages();
            PollConnectionStateChanges();
        }
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
        {
            SCX_CORE_ERROR("An error occured when checking for messages.");
            break;
        }

        auto packet_received = *static_cast<Packet*>(p_incoming_message->m_pData);

        // We use 0 for the 'from_client' parameter because on the client side we know
        // every packet comes from the server.
        m_handler.Handle(0, packet_received, &m_dispatcher);

        p_incoming_message->Release();
    }
}

void Client::PollConnectionStateChanges() const
{
    m_interface->RunCallbacks();
}

void Client::SendToServer(const Packet& data) const
{
    m_interface->SendMessageToConnection(m_connection, &data, sizeof(Packet),
                                         k_nSteamNetworkingSend_Reliable, nullptr);
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

void Client::OnConnectHandler(GameEvent& evt)
{
    const auto& on_connect_event = dynamic_cast<OnConnectEvent&>(evt);

    s_p_callback_instance->m_client_info.username = on_connect_event.username;
    s_p_callback_instance->Connect(on_connect_event.port, on_connect_event.ip);
}
