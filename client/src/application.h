#pragma once

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>

#include <memory>
#include <string>

class Window;

class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void Run();

private:
    std::unique_ptr<Window> m_window;
    HSteamNetConnection m_connection;
    ISteamNetworkingSockets* m_interface;

    void Initialise();
    void Dispose();

    void Connect(uint16_t port, const std::string& ip);
    void PollIncomingMessages() const;
    void PollConnectionStateChanges();

    void OnSteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info);

    static Application* s_p_callback_instance;

    /**
    * \brief The callback used when a connection status has been changed.
    * \param p_info Connection status callback information.
    */
    static void SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* p_info);
};
