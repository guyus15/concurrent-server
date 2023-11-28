#pragma once

#include <common/interface/iapplication.h>

#include <steam/isteamnetworkingsockets.h>

#include <string>
#include <unordered_map>

struct ServerSettings
{
    uint16_t port;
    int tick_rate;
    int max_no_clients;
};

class Application final : public IApplication
{
public:
    explicit Application(ServerSettings settings);
    ~Application() override;

    Application(const Application&) = default;
    Application& operator=(const Application&) = default;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void Run() override;

private:
    ServerSettings m_settings;
    ISteamNetworkingSockets* m_interface;
    HSteamListenSocket m_listen_socket;
    HSteamNetPollGroup m_poll_group;

    struct ClientInfo
    {
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
     * \brief Sends a message to the specified client.
     * \param message The string which will be dispatched to the client.
     * \param client_conn The client connection to which the message will be sent.
     */
    void SendMessageToClient(const std::string& message, HSteamNetConnection client_conn) const;

    /**
     * \brief Sends a message to all clients. If \code except\endcode is specified, the
     * associated client will be excluded from receiving the message.
     * \param message The string which will be dispatched to the clients.
     * \param except The client connection to exclude from the transmission.
     */
    void SendMessageToAllClients(const std::string& message,
                                 HSteamNetConnection except = k_HSteamNetConnection_Invalid);

    /**
     * \brief The callback used when a connection status has been changed, called on the
     * callback application instance.
     * \param p_info Connection status callback information.
     */
    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* p_info);

    static Application* s_p_callback_instance;

    /**
     * \brief The callback used when a connection status has been changed.
     * \param p_info Connection status callback information.
     */
    static void SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info);
};
