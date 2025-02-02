#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "client.h"
#include "input.h"

#include "ecs/components.h"
#include "ecs/entity.h"

#include "game/game.h"

#include "rendering/shader.h"

#include <common/assets/asset_manager.h>

#include <common/graphics/screen_manager.h>

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
      m_last_time{},
      m_connection{ k_HSteamNetConnection_Invalid },
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
    EventManager::AddListener<FrameBufferResizeEvent>(FrameBufferSizeHandler);
    EventManager::AddListener<OnConnectEvent>(OnConnectHandler);
    EventManager::AddListener<OnChatSendEvent>(OnChatSendHandler);
    EventManager::AddListener<OnLocalPlayerRespawnEvent>(OnLocalPlayerRespawnHandler);

    s_p_callback_instance = this;

#if defined(SCX_LOGGING)
    Logging::Initialise("CLIENT");
#endif

    if (!glfwInit())
        SCX_CORE_CRITICAL("Failed to initialise GLFW.");

    ScreenManager::Initialise();

    const std::string glsl_version = "#version 430";

    WindowSettings window_settings{};
    window_settings.title = "Client";
    window_settings.default_mode = WindowMode::Windowed;
    m_window = std::make_unique<Window>(window_settings);

    glfwSetKeyCallback(m_window->GetHandle(), KeyCallback);
    glfwSetCursorPosCallback(m_window->GetHandle(), MousePositionCallback);
    glfwSetMouseButtonCallback(m_window->GetHandle(), MouseButtonCallback);

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

    Game::Initialise();

    SCX_CORE_INFO("Application initialised.");
}

void Client::Run()
{
    const Shader& shader = AssetManager<Shader>::LoadOrRetrieve(
        "resources/shaders/vertex.glsl",
        "resources/shaders/fragment.glsl");

    glEnable(GL_BLEND);

    while (!m_window->ShouldClose())
    {
        const double current_time = glfwGetTime();
        const double delta_time = current_time - m_last_time;
        m_last_time = current_time;

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        shader.SetMat4x4("projection", Game::GetCamera().GetProjectionMatrix());

        Game::Update(delta_time);

        // UI begin
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        UiManager::Update(delta_time);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // UI end

        if (m_connection != k_HSteamListenSocket_Invalid)
        {
            PollIncomingMessages();
            PollConnectionStateChanges();

            if (Game::ShouldSendInput())
            {
                m_dispatcher.PlayerInput();
                m_dispatcher.PlayerWeaponRotation();
            }

            // Check for chat activation.
            if (Input::GetKeyDown(KeyCode::ForwardSlash))
            {
                OnChatVisibleEvent evt{};
                evt.visible = true;
                EventManager::Broadcast(evt);
            }
        }

        // Check for screen settings activation.
        if (Input::GetKeyDown(KeyCode::Escape))
        {
            OnScreenSettingsVisibleEvent evt{};
            EventManager::Broadcast(evt);
        }

        Input::Update();

        glfwPollEvents();
        m_window->SwapBuffers();
    }
}

void Client::SetClientId(const unsigned int id)
{
    s_p_callback_instance->m_client_info.id = id;
}

unsigned Client::GetClientId()
{
    return s_p_callback_instance->m_client_info.id;
}

void Client::Dispose()
{
    glfwTerminate();
    ShutdownSteamDatagramConnectionSockets();
}

void Client::Connect(const uint16_t port, const std::string& ip)
{
    SteamNetworkingIPAddr server_address{};

    m_connection = k_HSteamNetConnection_Invalid;

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
            {
                const std::string info = "Failed to connect to the remote host.";

                OnConnectStatusEvent on_connect_status_event{};
                on_connect_status_event.success = false;
                on_connect_status_event.info = info;

                EventManager::Broadcast(on_connect_status_event);

                SCX_CORE_ERROR(info);
            }
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

void Client::FrameBufferSizeHandler(GameEvent& evt)
{
    const auto& frame_buffer_size_event = dynamic_cast<FrameBufferResizeEvent&>(evt);

    glViewport(0, 0, frame_buffer_size_event.width, frame_buffer_size_event.height);

    ScreenManager::UpdateVideoModeResolution(frame_buffer_size_event.width, frame_buffer_size_event.height,
                                             frame_buffer_size_event.refresh_rate);
    Game::UpdateCamera();
}

void Client::OnConnectHandler(GameEvent& evt)
{
    const auto& on_connect_event = dynamic_cast<OnConnectEvent&>(evt);

    s_p_callback_instance->m_client_info.username = on_connect_event.username;
    s_p_callback_instance->Connect(static_cast<int16_t>(on_connect_event.port), on_connect_event.ip);
}

void Client::OnChatSendHandler(GameEvent& evt)
{
    const auto& on_chat_send_event = dynamic_cast<OnChatSendEvent&>(evt);

    s_p_callback_instance->m_dispatcher.SendChatMessage(on_chat_send_event.message);
}

void Client::OnLocalPlayerRespawnHandler(GameEvent& evt)
{
    s_p_callback_instance->m_dispatcher.PlayerRespawnRequest();
}
