#include "common/networking/networking.h"
#include "common/utils/logging.h"

void InitialiseNetworking()
{
    WSADATA wsa_data;

    if (const int i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data); i_result != 0)
        SCX_CORE_ERROR("WSAStartup() failed: {0}.", i_result);
}

void CleanupNetworking()
{
    if (const int i_result = WSACleanup(); i_result != 0)
        SCX_CORE_ERROR("WSACleanup() failed: {0}.", i_result);
}

int GetAddressInfo(const char* node_name, const char* service_name, const addrinfo* hints,
                   addrinfo** pp_result)
{
    const int i_result = getaddrinfo(node_name, service_name, hints, pp_result);
    if (i_result != 0)
    {
        SCX_CORE_ERROR("getaddrinfo() failed: {0}.", i_result);
        CleanupNetworking();
    }

    return i_result;
}

void FreeAddressInfo(addrinfo* address_info)
{
    freeaddrinfo(address_info);
}

unsigned long long CreateSocket(const int af, const int type, const int protocol)
{
    unsigned long long i_result = socket(af, type, protocol);
    if (i_result == INVALID_SOCKET)
    {
        SCX_CORE_ERROR("socket() failed: {0}.", i_result);
        CleanupNetworking();
    }

    return i_result;
}

int CloseSocket(const unsigned long long sock)
{
    const int i_result = closesocket(sock);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to close a socket: {0}.", WSAGetLastError());
        CleanupNetworking();
    }

    return i_result;
}

void Listen(const unsigned long long socket, const int backlog)
{
    const int i_result = listen(socket, backlog);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to listen on socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }
}

int Bind(const unsigned long long socket, const sockaddr* address_info, const int name_length)
{
    const int i_result = bind(socket, address_info, name_length);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to bind a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}

unsigned long long Accept(const unsigned long long socket, sockaddr* address_info, int* address_length)
{
    const unsigned long long i_result = accept(socket, address_info, address_length);
    if (i_result == INVALID_SOCKET)
    {
        SCX_CORE_ERROR("Failed to receive data from  a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}

int Receive(const unsigned long long socket, char* buffer, const int length, const int flags)
{
    const int i_result = recv(socket, buffer, length, flags);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to receive data from  a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}

int Send(const unsigned long long socket, const char* buffer, const int length, const int flags)
{
    const int i_result = send(socket, buffer, length, flags);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to send data on a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}

int Connect(const unsigned long long socket, const sockaddr* name, const int name_length)
{
    const int i_result = connect(socket, name, name_length);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to connect a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}

int Shutdown(const unsigned long long socket, const int how)
{
    const int i_result = shutdown(socket, how);
    if (i_result == SOCKET_ERROR)
    {
        SCX_CORE_ERROR("Failed to shutdown a socket: {0}.", WSAGetLastError());
        CloseSocket(socket);
        CleanupNetworking();
    }

    return i_result;
}
