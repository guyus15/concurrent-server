#pragma once

#include <common/interface/iapplication.h>

#include <steam/steamnetworkingsockets.h>

#include <string>

class Window;

class Client final : public IApplication
{
public:
    Client();
    ~Client() override;

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&&) noexcept = default;
    Client& operator=(Client&&) noexcept = default;

    void Run() override;

private:
    HSteamNetConnection m_connection;
    ISteamNetworkingSockets* m_interface;

    void Initialise() override;
    void Dispose() override;

    /**
     * \brief Attempts to create a connection to a server at \code ip\endcode on \code port\endcode.
     * \param port The port on which the server is running.
     * \param ip The address at which the server is running.
     */
    void Connect(uint16_t port, const std::string& ip);

    /**
     * \brief Polls incoming messages from the server.
     */
    void PollIncomingMessages() const;

    /**
     * \brief Polls connection state changes.
     */
    void PollConnectionStateChanges();

    /**
     * \brief The callback used when a connection status has been changed, called on the
     * application instance.
     * \param p_info Connection status callback information.
     */
    void OnSteamConnectionStatusChangedCallback(const SteamNetConnectionStatusChangedCallback_t* p_info);

    static Client* s_p_callback_instance;

    /**
    * \brief The callback used when a connection status has been changed.
    * \param p_info Connection status callback information.
    */
    static void SteamConnectionStatusChangedCallback(const SteamNetConnectionStatusChangedCallback_t* p_info);
};
