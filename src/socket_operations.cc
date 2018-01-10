//
// Created by Kévin POLOSSAT on 02/11/2017.
//

#include "socket_operations.h"

socket_type lw_network::socket_operations::socket(int domain, int type, int protocol, lw_network::error_code &e) {
#if defined (__linux__) || defined (__APPLE__)
    socket_type s = ::socket(domain, type, protocol);
    if (s == socket_error) {
        e = errno;
    }
    return s;
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

void lw_network::socket_operations::close(socket_type s, lw_network::error_code &e) {
#if defined (__linux__) || defined (__APPLE__)
    int ret = ::close(s);
    if (ret == socket_error) {
        e = errno;
    }
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
}

void lw_network::socket_operations::listen(socket_type s, int backlog, lw_network::error_code &e) {
#if defined (__linux__) || defined (__APPLE__)
    int ret = ::listen(s, backlog);
    if (ret == socket_error) {
        e = errno;
    }
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
}

signed_size_type lw_network::socket_operations::recv(
        socket_type s, Buffer *buff, std::size_t size, int flags, lw_network::error_code &e) {
#if defined (__linux__) || defined (__APPLE__)
    msghdr msg = {0};
    msg.msg_iov = buff;
    msg.msg_iovlen = static_cast<int>(size);
    signed_size_type ret = ::recvmsg(s, &msg, flags);
    if (ret == socket_error) {
        e = errno;
    }
    return ret;
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}

signed_size_type lw_network::socket_operations::send(
        socket_type s, Buffer *buff, std::size_t size, int flags, lw_network::error_code &e) {
#if defined (__linux__) || defined (__APPLE__)
    msghdr msg = {0};
    msg.msg_iov = buff;
    msg.msg_iovlen = static_cast<int>(size);
    signed_size_type ret = ::sendmsg(s, &msg, flags);
    if (ret == socket_error) {
        e = errno;
    }
    return ret;
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
    return 0;
}