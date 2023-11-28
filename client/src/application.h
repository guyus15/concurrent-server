#pragma once

#include <common/interface/iapplication.h>

#include <steam/steamnetworkingsockets.h>

#include <memory>
#include <string>

class Window;

class Application final : public IApplication
{
public:
    Application();
    ~Application() override;

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void Run() override;

private:
    std::unique_ptr<Window> m_window;
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

    static Application* s_p_callback_instance;

    /**
    * \brief The callback used when a connection status has been changed.
    * \param p_info Connection status callback information.
    */
    static void SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info);
};
