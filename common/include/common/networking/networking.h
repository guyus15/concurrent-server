/**
 * Functionality and documentation provided by the following:
 * - Windows Sockets 2 API: https://learn.microsoft.com/en-us/windows/win32/winsock/
 * - Linux: ...
 */

#pragma once

#ifdef SCX_WINDOWS_BUILD

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>

#pragma comment(lib,"WS2_32")

#endif

/**
 * \brief Initialises networking operations of the underlying libraries.
 */
void InitialiseNetworking();

/**
 * \brief Cleans up networking operations of the underlying libraries.
 */
void CleanupNetworking();

/**
 * \brief Provides protocol-independent translation from an ANSI hostname to an address.
 * \param node_name A pointer to a null-terminated ANSI string that contains a host name or a numeric host address string.
 * \param service_name A pointer to a null-terminated ANSI string that contains either a service name or port number represented as a string.
 * \param hints A pointer to an \code addrinfo\endcode structure that provides hints about the type of socket the caller supports.
 * \param pp_result A pointer to a linked list of one or more \code addrinfo\endcode structures that contains response information about the host.
 * \return Success returns zero. Failure returns a non-zero Windows Sockets error code.
 */
int GetAddressInfo(const char* node_name, const char* service_name, const addrinfo* hints,
                   addrinfo** pp_result);

/**
 * \brief Frees address information dynamically allocated from the \code getaddrinfo\endcode function.
 * \param address_info A pointer to a \code addrinfo\endcode structure of a linked list of \code addrinfo\endcode  structures to be freed.
 */
void FreeAddressInfo(addrinfo* address_info);

/**
 * \brief Creates a socket that is bound to a specific transport service provider.
 * \param af The address family specification.
 * \param type The type specification for the new socket.
 * \param protocol The protocol to be used by the socket.
 * \return A socket descriptor referencing the new socket.
 */
unsigned long long CreateSocket(int af, int type, int protocol);

/**
 * \brief Closes an existing socket.
 * \param socket A descriptor identifying the socket to close.
 * \return If no error occurs, zero is returned. Otherwise, a value of SOCKET_ERROR is returned.
 */
int CloseSocket(unsigned long long socket);

/**
 * \brief Places a socket in a state in which it is listening for an incoming connection.
 * \param socket A descriptor identifying a bound, unconnected socket.
 * \param backlog The maximum length of the queue of pending connections.
 */
void Listen(unsigned long long socket, int backlog);

/**
 * \brief Associates a local address with a socket.
 * \param socket A descriptor identifying an unbound socket.
 * \param name A pointer to a \code sockaddr\endcode structure of the local address to assign to the bound socket.
 * \param name_length The length, in bytes, of the value poitner to by \code address_info\endcode.
 * \return If no error occurs, zero is returned. Otherwise, a value of SOCKET_ERROR is returned.
 */
int Bind(unsigned long long socket, const sockaddr* address_info, int name_length);

/**
 * \brief Permits an incoming connection attempt on a socket.
 * \param socket A descriptor identifying a socket that has been placed in a listening state with the \code listen\endcode function.
 * \param address_info An optional pointer to a buffer that receives the address of the connecting entity, as known to the communication layer.
 * The exact format of the \code address_info\endcode parameter is determined by the address family that was established when the socket from the
 * \code sockaddr\endcode structure was created.
 * \param address_length An optional pointer to an integer that contains the length of the structure pointer to by the \code address_info\endcode parameter.
 * \return If no error occurs, the function returns a value that is a descriptor for the new socket. This returned value is a handle for the socket on which the actual connection is made.
 * Otherwise, a value of INVALID_SOCKET is returned.
 */
unsigned long long Accept(unsigned long long socket, sockaddr* address_info, int* address_length);

/**
 * \brief Receives data from a connected socket or a bound connectionless socket.
 * \param socket The descriptor identifying a connected socket.
 * \param buffer A pointer to the buffer to receive the incoming data.
 * \param length The length, in bytes, of the buffer pointer to by the \code buffer\endcode parameter.
 * \param flags A set of flags that influences the behaviour of this function.
 * \return If no error occurs, the function returned the number of bytes received and the buffer pointed to by the \code buffer\endcode
 * parameter will contain this data received. If the connection has been gracefully closed, zero is returned.
 * Otherwise, a value of SOCKET_ERROR is returned.
 */
int Receive(unsigned long long socket, char* buffer, int length, int flags);

/**
 * \brief Sends data on a connected socket.
 * \param socket A descriptor identifying a connected socket.
 * \param buffer A pointer to a buffer containing the data to be transmitted.
 * \param length The length, in bytes, of the data in a buffer pointer to by the \code buffer\endcode parameter.
 * \param flags A set of flags that influences the behaviour of this function.
 * \return If no error occurs, the total number of bytes sent is returned. Otherwise, a value of SOCKET_ERROR is returned.
 */
int Send(unsigned long long socket, const char* buffer, int length, int flags);

/**
 * \brief Establishes a connected to a specified socket.
 * \param socket A descriptor identifying an unconnected socket.
 * \param name A pointer to the \code sockaddr\endcode structure to which the connection should be established.
 * \param name_length The length, in bytes, of the \code sockaddr\endcode structure pointed to by the \code name\endcode parameter.
 * \return If no errors occur, zero is returned. Otherwise, a value of SOCKET_ERROR is returned.
 */
int Connect(unsigned long long socket, const sockaddr* name, int name_length);

/**
 * \brief Disables sends or receives on a socket.
 * \param socket A descriptor identifying a socket.
 * \param how A flag that describes what types of operation will no longer be allowed.
 * \return If no errors occur, zero is returned. Otherwise, a value of SOCKET_ERROR is returned.
 */
int Shutdown(unsigned long long socket, int how);
