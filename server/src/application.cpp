#include "application.h"

#include <common/networking/networking.h>
#include <common/utils/logging.h>

Application::Application(ServerSettings settings)
    : m_client_socket{}
{
    Initialise();
}

Application::~Application()
{
    Dispose();
}

void Application::Initialise()
{
    Logging::Initialise();

    InitialiseNetworking();

    addrinfo *result = nullptr, *ptr = nullptr, hints{};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    GetAddressInfo(nullptr, "27565", &hints, &result);

    const auto listen_socket = CreateSocket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listen_socket == INVALID_SOCKET)
        FreeAddressInfo(result);

    int i_result = 0;

    i_result = Bind(listen_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (i_result == SOCKET_ERROR)
        FreeAddressInfo(result);

    FreeAddressInfo(result);

    Listen(listen_socket, SOMAXCONN);

    m_client_socket = Accept(listen_socket, nullptr, nullptr);
}

void Application::Run() const
{
    int i_result = 0;
    char recv_buf[512]{};
    constexpr int recv_buf_len = 512;

    SCX_CORE_INFO("Server initialised.");

    do
    {

        i_result = Receive(m_client_socket, recv_buf, recv_buf_len, 0);
        if (i_result > 0)
        {
            SCX_CORE_INFO("Recieved Message (Bytes {0}): {1}", i_result, recv_buf);

            // Echo the buffer back to the sender.
            const int i_send_result = Send(m_client_socket, recv_buf, i_result, 0);
            if (i_send_result != SOCKET_ERROR)
                SCX_CORE_INFO("Sent Message (Bytes {0}): {1}", i_result, recv_buf);
        }
        else if (i_result == 0)
        {
            SCX_CORE_INFO("Connection closed.");
        }
    }
    while (i_result > 0);
}

void Application::Dispose() const
{
    const int i_result = Shutdown(m_client_socket, SD_SEND);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to shutdown: {0}", WSAGetLastError());
        CloseSocket(m_client_socket);
        WSACleanup();
    }

    // Cleanup
    CloseSocket(m_client_socket);
    WSACleanup();
}
