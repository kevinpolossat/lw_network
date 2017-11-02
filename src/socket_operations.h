//
// Created by Kévin POLOSSAT on 02/11/2017.
//

#ifndef LW_TCP_SERVER_SOCKET_DEF_H
#define LW_TCP_SERVER_SOCKET_DEF_H

#include <cstddef>
#include <sys/socket.h>

namespace network {
#if defined (__linux__) || defined (__APPLE__)

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using SOCKET = int;
using Buffer = iovec;
using signed_size_type = ssize_t;

#elif defined (_WIN32) || defined (_WIN64)

#error "TODO DEFINE WINDOWS"

#else

#error "unknown platform"

#endif
namespace socket_operations { // Low level socket function defined in according with the os.

static constexpr SOCKET invalid_socket = INVALID_SOCKET;
static constexpr int socket_error = SOCKET_ERROR;

SOCKET socket(int domain, int type, int protocol);
int close(SOCKET s);
template<typename SockLenType>
int bind(SOCKET s, const struct sockaddr *addr, std::size_t addrlen);
int listen(SOCKET s, int backlog);
template<typename SockLenType>
int connect(SOCKET s, const struct sockaddr *addr, std::size_t addrlen);
template<typename SockLenType>
int accept(SOCKET s, struct sockaddr *addr, std::size_t addrlen);
signed_size_type recv(SOCKET s, Buffer *buff, std::size_t size, int flags);
signed_size_type send(SOCKET s, Buffer *buff, std::size_t size, int flags);
} // socket_operations
} // network

#endif //LW_TCP_SERVER_SOCKET_DEF_H
