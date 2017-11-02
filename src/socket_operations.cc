//
// Created by Kévin POLOSSAT on 02/11/2017.
//

#include "socket_operations.h"

network::SOCKET network::socket_operations::socket(int domain, int type, int protocol) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

int network::socket_operations::close(network::SOCKET s) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

template<typename SockLenType>
int network::socket_operations::bind(network::SOCKET s, const struct sockaddr *addr, std::size_t addrlen) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

int network::socket_operations::listen(network::SOCKET s, int backlog) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

template<typename SockLenType>
int network::socket_operations::connect(network::SOCKET s, const struct sockaddr *addr, std::size_t addrlen) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

template<typename SockLenType>
int network::socket_operations::accept(network::SOCKET s, struct sockaddr *addr, std::size_t addrlen) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

network::signed_size_type network::socket_operations::recv(
        network::SOCKET s, Buffer *buff, std::size_t size, int flags) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

network::signed_size_type network::socket_operations::send(
        SOCKET s, Buffer *buff, std::size_t size, int flags) {
#if defined (__linux__) || defined (__APPLE__)
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}
