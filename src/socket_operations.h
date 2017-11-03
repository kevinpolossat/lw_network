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

using socket_type = int;
using Buffer = iovec;
using signed_size_type = ssize_t;

#elif defined (_WIN32) || defined (_WIN64)

#error "TODO DEFINE WINDOWS"

#else

#error "unknown platform"

#endif

static constexpr socket_type invalid_socket = INVALID_SOCKET;
static constexpr int socket_error = SOCKET_ERROR;
namespace network {
using error_code = int;
static constexpr error_code no_error = 0;

namespace socket_operations { // Low level socket function defined in according with the os.

socket_type socket(int domain, int type, int protocol, network::error_code &e);

void close(socket_type s, network::error_code &e);

template<typename SockLenType>
void bind(socket_type s, const struct sockaddr *addr, std::size_t addrlen, network::error_code &e);

void listen(socket_type s, int backlog, network::error_code &e);

template<typename SockLenType>
void connect(socket_type s, const struct sockaddr *addr, std::size_t addrlen, network::error_code &e);

template<typename SockLenType>
socket_type accept(socket_type s, struct sockaddr *addr, std::size_t *addrlen, network::error_code &e);

signed_size_type recv(socket_type s, Buffer *buff, std::size_t size, int flags, network::error_code &e);

signed_size_type send(socket_type s, Buffer *buff, std::size_t size, int flags, network::error_code &e);

template<typename SockLenType>
void setsockopt(socket_type s, int level, int optname, void const *optval, std::size_t optlen, network::error_code &e);

template<typename SockLenType>
void getsockopt(socket_type s, int level, int optname, void *optval, std::size_t *optlen, network::error_code &e);

} // socket_operations
} // network

#endif //LW_TCP_SERVER_SOCKET_DEF_H
