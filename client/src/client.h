#pragma once

#include "client_packet_handler.h"
#include "client_packet_dispatcher.h"

#include <common/events/event_manager.h>

#include <common/interface/iapplication.h>

#include <common/utils/clock.h>

#include <steam/steamnetworkingsockets.h>

#include <string>

class Window;

struct ClientInfo
{
    unsigned int id{ 0 };
    std::string username;
};

/**
 * \brief Implementation of \code IApplication\endcode to represent the client application.
 */
class Client final : public IApplication
{
public:
    Client();
    ~Client() override;

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    Client(Client&&) noexcept = delete;
    Client& operator=(Client&&) noexcept = delete;

    void Run() override;

    /**
     * \brief Sets the client's network ID.
     * \param id The new client network ID.
     */
    static void SetClientId(unsigned int id);

    /**
     * \brief Gets the client's network ID.
     * \return The client's network ID. 
     */
    static [[nodiscard]] unsigned int GetClientId();

private:
    ClientPacketHandler m_handler;
    ClientPacketDispatcher m_dispatcher;
    ClientInfo m_client_info;
    Clock m_clock;
    double m_last_time;

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
    void PollConnectionStateChanges() const;

    /**
     * \brief Sends a packet to the connected server.
     * \param data The packet which will be dispatched to the server.
     */
    void SendToServer(const Packet& data) const;

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

    /**
     * \brief The handler used when the frame buffer has been resized.
     * \param evt Frame buffer size event details.
     */
    static void FrameBufferSizeHandler(GameEvent& evt);

    /**
     * \brief The handler used when an \code OnConnectEvent\endcode occurs.
     * \param evt The event.
     */
    static void OnConnectHandler(GameEvent& evt);

    /**
     * \brief The handler used when an \code OnChatSendEvent\endcode occurs.
     * \param evt The event.
     */
    static void OnChatSendHandler(GameEvent& evt);

    /**
     * \brief The handler used when an \code OnLocalPlayerRespawnEvent\endcode occurs.
     * \param evt The event.
     */
    static void OnLocalPlayerRespawnHandler(GameEvent& evt);

    friend class ClientPacketDispatcher;
};
