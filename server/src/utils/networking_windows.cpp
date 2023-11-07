#include "networking.h"

#define SCX_WINDOWS_BUILD
#ifdef SCX_WINDOWS_BUILD

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#endif

int GetAddressInfo(const char* node_name, const char* service_name, const addrinfo* hints,
                   addrinfo** pp_result)
{
    return getaddrinfo(node_name, service_name, hints, pp_result);
}

void FreeAddressInfo(addrinfo* address_info)
{
    freeaddrinfo(address_info);
}

void CloseSocket(const unsigned long long socket)
{
    closesocket(socket);
}

int Listen(const unsigned long long socket, const int backlog)
{
    return listen(socket, backlog);
}

int Bind(const unsigned long long socket, const sockaddr* name, const int name_length)
{
    return bind(socket, name, name_length);
}

unsigned long long Accept(const unsigned long long socket, sockaddr* addr, int* address_length)
{
    return accept(socket, addr, address_length);
}

int Receive(const unsigned long long socket, char* buffer, const int length, const int flags)
{
    return recv(socket, buffer, length, flags);
}

int Send(const unsigned long long socket, const char* buf, const int len, const int flags)
{
    return send(socket, buf, len, flags);
}

int Shutdown(const unsigned long long socket, const int how)
{
    return shutdown(socket, how);
}