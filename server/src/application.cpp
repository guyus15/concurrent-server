#include "application.h"

#include <common/utils/assertion.h>
#include <common/utils/logging.h>
#include <common/utils/networking.h>

#include <steam/steamnetworkingsockets.h>

#include <sstream>
#include <ranges>

Application *Application::s_p_callback_instance = nullptr;

Application::Application(const ServerSettings settings)
    : m_settings{settings},
      m_interface{nullptr},
      m_listen_socket{k_HSteamListenSocket_Invalid},
      m_poll_group{k_HSteamNetPollGroup_Invalid}
{
    Initialise();
}

Application::~Application()
{
    Dispose();
}

void Application::Initialise()
{
    Logging::Initialise("SERVER");

    InitialiseSteamDatagramConnectionSockets();

    // Select interface instance to use.
    m_interface = SteamNetworkingSockets();
}

void Application::Run()
{
    SteamNetworkingIPAddr server_local_address{};
    server_local_address.m_port = m_settings.port;

    SteamNetworkingConfigValue_t option{};
    option.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                  reinterpret_cast<void *>(SteamConnectionStatusChangedCallback));

    m_listen_socket = m_interface->CreateListenSocketIP(server_local_address, 1, &option);
    if (m_listen_socket == k_HSteamListenSocket_Invalid)
        SCX_CORE_ERROR("Failed to listen on port {0}.", m_settings.port);

    m_poll_group = m_interface->CreatePollGroup();
    if (m_poll_group == k_HSteamNetPollGroup_Invalid)
        SCX_CORE_ERROR("Failed to listen on port {0}.", m_settings.port);

    SCX_CORE_INFO("Server listening on port {0}.", m_settings.port);

    const int sleep_duration = 1000 / m_settings.tick_rate;

    // TODO: make this a boolean that can be set to false to break out of loop.
    bool running = true;
    while (running)
    {
        PollIncomingMessages();
        PollConnectionStateChanges();

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
    }
}

void Application::Dispose()
{
    SCX_CORE_INFO("Closing connections to server.");

    for (const auto client : std::views::keys(m_clients))
    {
        // Send a farewell message to each client.
        SendMessageToClient("Connection to the server closed.", client);

        // Close the connection. "linger mode" asks SteamNetworkingSockets to flush this connection
        // and terminate gracefully.
        m_interface->CloseConnection(client, 0, "Server shutdown", true);
    }

    m_clients.clear();

    m_interface->CloseListenSocket(m_listen_socket);
    m_listen_socket = k_HSteamListenSocket_Invalid;

    m_interface->DestroyPollGroup(m_poll_group);
    m_poll_group = k_HSteamNetPollGroup_Invalid;

    ShutdownSteamDatagramConnectionSockets();
}

void Application::PollIncomingMessages()
{
    while (true)
    {
        ISteamNetworkingMessage *p_incoming_message = nullptr;

        const int num_msgs = m_interface->ReceiveMessagesOnPollGroup(m_poll_group, &p_incoming_message, 1);
        if (num_msgs == 0)
            break;
        if (num_msgs < 0)
            SCX_CORE_ERROR("An error occurred when checking for messages.");

        // Check that a valid message exists.
        SCX_ASSERT(num_msgs == 1 && p_incoming_message, "A valid message does not exist.");

        auto it_client = m_clients.find(p_incoming_message->m_conn);
        SCX_ASSERT(it_client != m_clients.end(), "There isn't a client associated with the incoming message.");

        // Copy the contents of the message into a '\0'-terminated string.
        std::string message_contents;
        message_contents.assign(static_cast<const char *>(p_incoming_message->m_pData), p_incoming_message->m_cbSize);

        // Release resource handle to the incoming message now it has been copied.
        p_incoming_message->Release();

        // Display and dispatch the message to everybody else.
        SCX_CORE_INFO("Message ({0}): {1}", it_client->second.name, message_contents);
        SendMessageToAllClients(message_contents, it_client->first);
    }
}

void Application::PollConnectionStateChanges()
{
    s_p_callback_instance = this;
    m_interface->RunCallbacks();
}

void Application::SendMessageToClient(const std::string &message, const HSteamNetConnection client_conn) const
{
    m_interface->SendMessageToConnection(client_conn, message.c_str(), static_cast<uint32_t>(message.length()),
                                         k_nSteamNetworkingSend_Reliable, nullptr);
}

void Application::SendMessageToAllClients(const std::string &message, const HSteamNetConnection except)
{
    for (const auto &conn : std::views::keys(m_clients))
    {
        if (conn != except)
            SendMessageToClient(message, conn);
    }
}

void Application::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *p_info)
{
    // Determine the state of the server's connection.
    switch (p_info->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_None:
        break;
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        // The following will be ignored if the client was not previously connected or if a client disconnects
        // before their connection was accepted by the server.
        if (p_info->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
        {
            const auto it_client = m_clients.find(p_info->m_hConn);
            SCX_ASSERT(it_client != m_clients.end(), "There isn't a client associated with this connection.");

            std::string error_log;

            if (p_info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                error_log = "problem detected locally";
            else
                error_log = "closed by peer";

            // Log on our side.
            SCX_CORE_INFO("Connection {0} {1}: {2} {3}", p_info->m_info.m_szConnectionDescription, error_log,
                          p_info->m_info.m_eEndReason, p_info->m_info.m_szEndDebug);

            // Send a message to inform other clients.
            std::stringstream farewell_msg_stream;
            farewell_msg_stream << it_client->second.name << " has disconnected from the server (" << error_log << ").";

            m_clients.erase(it_client);

            SendMessageToAllClients(farewell_msg_stream.str());
        }
        else
            assert(p_info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);

        // Cleanup the connection.
        m_interface->CloseConnection(p_info->m_hConn, 0, nullptr, false);
        break;
    }
    case k_ESteamNetworkingConnectionState_Connecting:
    {
        // Make sure this is a new connection by checking existing clients.
        SCX_ASSERT(!m_clients.contains(p_info->m_hConn), "A client associated with this connection already exists.");

        SCX_CORE_INFO("Connection request from {0}.", p_info->m_info.m_szConnectionDescription);

        // A client is attempting to connect, so try to accept the connection.
        if (m_interface->AcceptConnection(p_info->m_hConn) != k_EResultOK)
        {
            m_interface->CloseConnection(p_info->m_hConn, 0, nullptr, false);
            SCX_CORE_ERROR("Cannot accept connection (was it already closed?).");
            break;
        }

        // Attempt to assign the connection to a poll group.
        if (!m_interface->SetConnectionPollGroup(p_info->m_hConn, m_poll_group))
        {
            m_interface->CloseConnection(p_info->m_hConn, 0, nullptr, false);
            SCX_CORE_ERROR("Failed to set poll group for connection.");
            break;
        }

        // PLACEHOLDER: Generate a random name for the client.
        // In the future, I will let the client assign their own name upon connection.
        std::stringstream name_stream;
        name_stream << "SomeName#" << 10000 + (rand() % 100000);

        // Send a welcome message to the new client.
        std::stringstream welcome_msg_stream;
        welcome_msg_stream << "Welcome to the server, " << name_stream.str() << "!";

        SendMessageToClient(welcome_msg_stream.str(), p_info->m_hConn);

        // Send the new client a list of everybody already connected.
        if (m_clients.empty())
            SendMessageToClient("You are currently here alone.", p_info->m_hConn);
        else
        {
            std::stringstream clients_msg_stream;
            clients_msg_stream << "You are joined by " << m_clients.size() << " others:";
            SendMessageToClient(clients_msg_stream.str(), p_info->m_hConn);

            for (const auto &client : std::views::values(m_clients))
                SendMessageToClient(client.name, p_info->m_hConn);
        }

        // Let other clients know of the new client.
        std::stringstream client_msg_stream;
        client_msg_stream << name_stream.str() << " has joined the server!";
        SendMessageToAllClients(client_msg_stream.str(), p_info->m_hConn);

        // Add the new client to client list.
        m_clients[p_info->m_hConn].name = name_stream.str();
        break;
    }
    default:
        // We are not concerned with other connection status callback states.
        break;
    }
}

void Application::SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *p_info)
{
    s_p_callback_instance->OnSteamNetConnectionStatusChanged(p_info);
}
