#pragma once

#define SCX_WINDOWS_BUILD
#ifdef SCX_WINDOWS_BUILD

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>

#pragma comment(lib,"WS2_32")

#endif

int GetAddressInfo(const char* node_name, const char* service_name, const addrinfo* hints,
                   addrinfo** pp_result);
void FreeAddressInfo(addrinfo* address_info);
void CloseSocket(unsigned long long socket);
int Listen(unsigned long long socket, int backlog);
int Bind(unsigned long long socket, const sockaddr* name, int name_length);
unsigned long long Accept(unsigned long long socket, sockaddr* addr, int* address_length);
int Receive(unsigned long long socket, char* buffer, int length, int flags);
int Send(unsigned long long socket, const char* buf, int len, int flags);
int Shutdown(unsigned long long socket, int how);
