//
// Created by Kévin POLOSSAT on 02/11/2017.
//

#ifndef LW_TCP_SERVER_SOCKET_DEF_H
#define LW_TCP_SERVER_SOCKET_DEF_H

#if defined (__linux__) || defined (__APPLE__)

#include <cstddef>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>

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

static constexpr SOCKET invalid_socket = INVALID_SOCKET;
static constexpr int socket_error = SOCKET_ERROR;
namespace network {
using error_code = int;
static constexpr error_code no_error = 0;

namespace socket_operations { // Low level socket function defined in according with the os.

SOCKET socket(int domain, int type, int protocol, network::error_code &e);
void close(SOCKET s, network::error_code &e);
template<typename SockLenType>
void bind(SOCKET s, const struct sockaddr *addr, std::size_t addrlen, network::error_code &e);
void listen(SOCKET s, int backlog, network::error_code &e);
template<typename SockLenType>
void connect(SOCKET s, const struct sockaddr *addr, std::size_t addrlen, network::error_code &e);
template<typename SockLenType>
SOCKET accept(SOCKET s, struct sockaddr *addr, std::size_t *addrlen, network::error_code &e);
signed_size_type recv(SOCKET s, Buffer *buff, std::size_t size, int flags, network::error_code &e);
signed_size_type send(SOCKET s, Buffer *buff, std::size_t size, int flags, network::error_code &e);
} // socket_operations
} // network

#endif //LW_TCP_SERVER_SOCKET_DEF_H
