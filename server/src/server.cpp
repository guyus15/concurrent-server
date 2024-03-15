#include "server.h"
#include "game.h"
#include "thread_pool.h"

#include <common/assets/asset_manager.h>

#include <common/networking/core.h>

#include <common/utils/assertion.h>
#include <common/utils/logging.h>

#include <steam/steamnetworkingsockets.h>

#include <ranges>

Server* Server::s_p_callback_instance = nullptr;

Server::Server(const ServerSettings settings)
    : m_settings{ settings },
      m_dispatcher{ this },
      m_interface{ nullptr },
      m_listen_socket{ k_HSteamListenSocket_Invalid },
      m_poll_group{ k_HSteamNetPollGroup_Invalid }
{
    Initialise();
}

Server::~Server()
{
    Dispose();
}

void Server::Initialise()
{
#if defined(SCX_LOGGING)
    Logging::Initialise("SERVER");
#endif

    InitialiseSteamDatagramConnectionSockets();

    // Select interface instance to use.
    m_interface = SteamNetworkingSockets();

    ThreadPool::Initialise(m_handler, m_dispatcher);

    Game::Initialise();
}

void Server::Run()
{
    SteamNetworkingIPAddr server_local_address{};
    server_local_address.m_port = m_settings.port;

    SteamNetworkingConfigValue_t option{};
    option.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,
                  reinterpret_cast<void*>(SteamConnectionStatusChangedCallback));

    m_listen_socket = m_interface->CreateListenSocketIP(server_local_address, 1, &option);
    if (m_listen_socket == k_HSteamListenSocket_Invalid)
        SCX_CORE_ERROR("Failed to listen on port {0}.", m_settings.port);

    m_poll_group = m_interface->CreatePollGroup();
    if (m_poll_group == k_HSteamNetPollGroup_Invalid)
        SCX_CORE_ERROR("Failed to listen on port {0}.", m_settings.port);

    SCX_CORE_INFO("Server listening on port {0}.", m_settings.port);

    const int sleep_duration = 1000 / m_settings.tick_rate;

    while (true)
    {
        const auto current_time = std::chrono::high_resolution_clock::now();
        const double delta_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - m_last_time).count()) / 1000;
        m_last_time = current_time;

        PollIncomingMessages();
        PollConnectionStateChanges();

        Game::Update(delta_time);

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
    }
}

std::unordered_map<HSteamNetConnection, ClientInfo>& Server::GetClientInfoMap()
{
    return s_p_callback_instance->m_client_info;
}

std::unordered_map<HSteamNetConnection, UUID>& Server::GetClientThreadMap()
{
    return s_p_callback_instance->m_client_threads;
}

void Server::Dispose()
{
    SCX_CORE_INFO("Closing connections to server.");

    for (const auto client : std::views::keys(m_client_info))
    {
        // Send a farewell packet message to each client.
        Packet farewell_packet{ PacketType::ServerShutdown };
        SendToClient(farewell_packet, client);

        // Close the connection. "linger mode" asks SteamNetworkingSockets to flush this connection
        // and terminate gracefully.
        m_interface->CloseConnection(client, 0, "Server shutdown", true);
    }

    ThreadPool::Dispose();

    m_client_info.clear();
    m_client_threads.clear();

    m_interface->CloseListenSocket(m_listen_socket);
    m_listen_socket = k_HSteamListenSocket_Invalid;

    m_interface->DestroyPollGroup(m_poll_group);
    m_poll_group = k_HSteamNetPollGroup_Invalid;

    ShutdownSteamDatagramConnectionSockets();
}

void Server::PollIncomingMessages()
{
    while (true)
    {
        ISteamNetworkingMessage* p_incoming_message = nullptr;

        const int num_msgs = m_interface->ReceiveMessagesOnPollGroup(m_poll_group, &p_incoming_message, 1);
        if (num_msgs == 0)
            break;
        if (num_msgs < 0)
            SCX_CORE_ERROR("An error occurred when checking for messages.");

        // Check that a valid message exists.
        SCX_ASSERT(num_msgs == 1 && p_incoming_message, "A valid message does not exist.");

        auto it_client = m_client_info.find(p_incoming_message->m_conn);
        SCX_ASSERT(it_client != m_client_info.end(), "There isn't a client associated with the incoming message.");

        auto packet_received = *static_cast<Packet*>(p_incoming_message->m_pData);

        // Add the packet to a queue to be processed by the relevant thread.
        ThreadPool::EnqueuePacketToHandle(packet_received, p_incoming_message->m_conn);

        p_incoming_message->Release();
    }
}

void Server::PollConnectionStateChanges()
{
    s_p_callback_instance = this;
    m_interface->RunCallbacks();
}

void Server::SendToClient(const Packet& data, const HSteamNetConnection client_conn) const
{
    m_interface->SendMessageToConnection(client_conn, &data, sizeof(Packet),
                                         k_nSteamNetworkingSend_Reliable, nullptr);
}

void Server::SendToAllClients(const Packet& data, const HSteamNetConnection except) const
{
    for (const auto& conn : std::views::keys(m_client_info))
    {
        if (conn != except)
            SendToClient(data, conn);
    }
}

void Server::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* p_info)
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
                const auto it_client_info = m_client_info.find(p_info->m_hConn);
                SCX_ASSERT(it_client_info != m_client_info.end(),
                           "There isn't any client information associated with this connection.");

                const auto it_client_thread = m_client_threads.find(p_info->m_hConn);
                SCX_ASSERT(it_client_thread != m_client_threads.end(),
                           "There isn't a thread associated with this connection.");

                std::string error_log;

                if (p_info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
                    error_log = "problem detected locally";
                else
                    error_log = "closed by peer";

                // Log on server side.
                SCX_CORE_INFO("{0} has disconnected from the server ({1}).", it_client_info->second.username,
                              error_log);

                // Inform other clients that this client has disconnected.
                PlayerDisconnected(p_info->m_hConn, it_client_info->second.username);

                // Cleanup
                ThreadPool::TerminateThread(it_client_thread->second);

                m_client_info.erase(it_client_info);
                m_client_threads.erase(it_client_thread);
            }
            else
                SCX_ASSERT(p_info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting,
                       "Unexpected old state.");

            // Cleanup the connection.
            m_interface->CloseConnection(p_info->m_hConn, 0, nullptr, false);
            break;
        }
    case k_ESteamNetworkingConnectionState_Connecting:
        {
            // Make sure this is a new connection by checking existing clients.
            SCX_ASSERT(!m_client_info.contains(p_info->m_hConn),
                       "A client associated with this connection already exists.");

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

            // Allocate a thread to manage this connection.
            const UUID thread_id = ThreadPool::AllocateThread();

            // Add the new client to the client lists.
            // Note: Due to the dependency on an allocated thread to manage each connection,
            // this the client must be associated with a thread before any packets can be sent
            // or received.
            m_client_info[p_info->m_hConn].username = "PlaceholderUsername";
            m_client_threads[p_info->m_hConn] = thread_id;

            // Send a welcome message to the new client.
            Welcome(p_info->m_hConn, "Welcome to the server.");

            break;
        }
    default:
        // We are not concerned with other connection status callback states.
        break;
    }
}

void Server::SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info)
{
    s_p_callback_instance->OnSteamNetConnectionStatusChanged(p_info);
}
