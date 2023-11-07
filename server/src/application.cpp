#include "application.h"

#include <common/networking/networking.h>

#include <iostream>

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

    do
    {
        std::cout << "Running...\n";

        i_result = Receive(m_client_socket, recv_buf, recv_buf_len, 0);
        if (i_result > 0)
        {
            std::cout << "Bytes received: " << i_result << "\nMessage: " << recv_buf << "\n";

            // Echo the buffer back to the sender.
            const int i_send_result = Send(m_client_socket, recv_buf, i_result, 0);
            if (i_send_result != SOCKET_ERROR)
                std::cout << "Bytes sent: " << i_send_result << "\n";
        }
        else if (i_result == 0)
        {
            std::cout << "Connection closing...\n";
        }
    }
    while (i_result > 0);
}

void Application::Dispose() const
{
    const int i_result = Shutdown(m_client_socket, SD_SEND);
    if (i_result == SOCKET_ERROR)
    {
        std::cout << "Failed to shutdown: " << WSAGetLastError() << "\n";
        CloseSocket(m_client_socket);
        WSACleanup();
    }

    // Cleanup
    CloseSocket(m_client_socket);
    WSACleanup();
}
