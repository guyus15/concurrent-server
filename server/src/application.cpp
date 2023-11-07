#include "application.h"

#include "common/networking/networking.h"

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
    WSADATA wsa_data;

    int i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (i_result != 0)
    {
        std::cout << "WSAStartup failed: " << i_result << "\n";
    }

    addrinfo *result = nullptr, *ptr = nullptr, hints{};

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    i_result = GetAddressInfo(nullptr, "27565", &hints, &result);
    if (i_result != 0)
    {
        std::cout << "GetAddressInfo failed: " << i_result << "\n";
        WSACleanup();
    }

    const auto listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_socket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << "\n";
        FreeAddressInfo(result);
        WSACleanup();
    }

    i_result = bind(listen_socket, result->ai_addr, static_cast<int>(result->ai_addrlen));
    if (i_result == SOCKET_ERROR)
    {
        std::cout << "Error at bind(): " << WSAGetLastError() << "\n";
        FreeAddressInfo(result);
        CloseSocket(listen_socket);
        WSACleanup();
    }

    FreeAddressInfo(result);

    if (Listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Error at listen(): " << WSAGetLastError() << "\n";
        CloseSocket(listen_socket);
        WSACleanup();
    }

    m_client_socket = Accept(listen_socket, nullptr, nullptr);
    if (m_client_socket == INVALID_SOCKET)
    {
        std::cout << "Failed to accept client socket: " << WSAGetLastError() << "\n";
        CloseSocket(listen_socket);
        WSACleanup();
    }
}

void Application::Run() const
{
    int i_result = 0;
    char recv_buf[512]{};
    constexpr int recv_buf_len = 512;

    do
    {
        std::cout << "Running...\n";

        i_result = recv(m_client_socket, recv_buf, recv_buf_len, 0);
        if (i_result > 0)
        {
            std::cout << "Bytes received: " << i_result << "\nMessage: " << recv_buf << "\n";

            // Echo the buffer back to the sender.
            const int i_send_result = Send(m_client_socket, recv_buf, i_result, 0);
            if (i_send_result == SOCKET_ERROR)
            {
                std::cout << "Failed to send: " << WSAGetLastError() << "\n";
                CloseSocket(m_client_socket);
                WSACleanup();
            }

            std::cout << "Bytes sent: " << i_send_result << "\n";
        }
        else if (i_result == 0)
        {
            std::cout << "Connection closing...\n";
        }
        else
        {
            std::cout << "Failed to receive: " << WSAGetLastError() << "\n";
            CloseSocket(m_client_socket);
            WSACleanup();
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
