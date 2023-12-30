#pragma once

#include "server_packet_dispatcher.h"
#include "server_packet_handler.h"

#include <common/interface/iapplication.h>

#include <common/networking/packet.h>

#include <common/utils/clock.h>
#include <common/utils/uuid.h>

#include <steam/isteamnetworkingsockets.h>

#include <string>
#include <unordered_map>

struct ServerSettings
{
    uint16_t port;
    int tick_rate;
    int max_no_clients;
};

/**
 * \brief Implementation of \code IApplication\endcode to represent the server application.
 */
class Server final : public IApplication
{
public:
    explicit Server(ServerSettings settings);
    ~Server() override;

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    Server(Server&&) noexcept = delete;
    Server& operator=(Server&&) noexcept = delete;

    void Run() override;

private:
    Clock m_server_clock;
    ServerSettings m_settings;

    // Move the server packet handler and dispatcher to the thread pool as this is where
    // packets will be handled.
    ServerPacketHandler m_handler;
    ServerPacketDispatcher m_dispatcher;
    ISteamNetworkingSockets* m_interface;
    HSteamListenSocket m_listen_socket;
    HSteamNetPollGroup m_poll_group;

    struct ClientInfo
    {
        UUID thread;
        std::string name;
    };

    std::unordered_map<HSteamNetConnection, ClientInfo> m_clients;

    void Initialise() override;
    void Dispose() override;

    /**
     * \brief Polls incoming messages from clients.
     */
    void PollIncomingMessages();

    /**
     * \brief Polls connection state changes.
     */
    void PollConnectionStateChanges();

    /**
     * \brief Sends a packet to the specified client.
     * \param data The packet which will be dispatched to the client.
     * \param client_conn The client connection to which the packet will be sent.
     */
    void SendToClient(const Packet& data, HSteamNetConnection client_conn) const;

    /**
     * \brief Sends a packet to all clients. If \code except\endcode is specified, the
     * associated client will be excluded from receiving the packet.
     * \param data The packet which will be dispatched to each client.
     * \param except The client connection to exclude from the transmission.
     */
    void SendToAllClients(const Packet& data, HSteamNetConnection except = k_HSteamListenSocket_Invalid) const;

    /**
     * \brief The callback used when a connection status has been changed, called on the
     * callback application instance.
     * \param p_info Connection status callback information.
     */
    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* p_info);

    static Server* s_p_callback_instance;

    /**
     * \brief The callback used when a connection status has been changed.
     * \param p_info Connection status callback information.
     */
    static void SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info);

    friend class ServerPacketDispatcher;
};
