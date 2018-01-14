//
// Created by Kévin POLOSSAT on 08/01/2018.
//

#include "BasicSocket.h"

lw_network::BasicSocket::BasicSocket(socket_type s): s_(s) {}

lw_network::BasicSocket::~BasicSocket() {}

lw_network::BasicSocket::BasicSocket(lw_network::BasicSocket &&other) {
    s_ = other.s_;
    other.s_ = invalid_socket;
}

lw_network::BasicSocket &lw_network::BasicSocket::operator=(lw_network::BasicSocket &&other) {
    s_ = other.s_;
    other.s_ = invalid_socket;
    return *this;
}

lw_network::BasicSocket::BasicSocket(const lw_network::BasicSocket &other): s_(other.s_) {}

lw_network::BasicSocket &lw_network::BasicSocket::operator=(const lw_network::BasicSocket &other) {
    s_ = other.s_;
    return *this;
}

lw_network::BasicSocket &lw_network::BasicSocket::operator=(socket_type s) {
    s_ = s;
    return *this;
}

void lw_network::BasicSocket::open(const lw_network::Protocol &proto, error_code &e) {
    s_ = socket_operations::socket(proto.family(), proto.type(), proto.protocol(), e);
}

void lw_network::BasicSocket::close(error_code &e) {
    socket_operations::close(s_, e);
    s_ = invalid_socket;
}

void lw_network::BasicSocket::bind(const lw_network::EndPoint &endpoint, error_code &e) {
    socket_operations::bind<SockLenType>(s_, endpoint.Data(), endpoint.Size(), e);
}

void lw_network::BasicSocket::listen(int backlog, error_code &e) { socket_operations::listen(s_, backlog, e); }

void lw_network::BasicSocket::connect(const lw_network::EndPoint &endPoint, error_code &e) {
    socket_operations::connect<SockLenType>(s_, endPoint.Data(), endPoint.Size(), e);
}
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void lw_network::BasicSocket::accept(lw_network::BasicSocket &socket, error_code &e) {
    EndPoint endPoint;
    std::size_t size = endPoint.Size();
    socket = socket_operations::accept<SockLenType>(s_, endPoint.Data(), &size, e);
//    char remoteIP[INET6_ADDRSTRLEN];
//    printf("new connection from %s\n", inet_ntop(endPoint.Data()->sa_family, get_in_addr(endPoint.Data()), remoteIP, INET6_ADDRSTRLEN));
}

void lw_network::BasicSocket::setOption(int level, int optname, void const *optval, std::size_t optlen, error_code &e) {
    socket_operations::setsockopt<SockLenType>(s_, level, optname, optval, optlen, e);
}

void lw_network::BasicSocket::getOption(int level, int optname, void *optval, std::size_t *optlen, error_code &e) const {
    socket_operations::getsockopt<SockLenType>(s_, level, optname, optval, optlen, e);
}

signed_size_type lw_network::BasicSocket::recv(Buffer &buffer, int flags, error_code &e) {
    io_buffer b = {buffer.Data(), buffer.Size()};
    auto s = socket_operations::recv(getImpl(), &b, buffer.Size(), flags, e);
    if (!e) {
        buffer += s;
    }
    return s;
}

signed_size_type lw_network::BasicSocket::send(Buffer &buffer, int flags, error_code &e) {
    io_buffer b = {const_cast<void *>(buffer.Data()), buffer.Size()};
    auto s = socket_operations::send(getImpl(), &b, buffer.Size(), flags, e);
    if (!e) {
        buffer += s;
    }
    return s;
}

socket_type lw_network::BasicSocket::getImpl() const { return s_; }

lw_network::EndPoint lw_network::BasicSocket::localEndPoint(lw_network::error_code &e) const {
    EndPoint ep;

    std::size_t s = ep.Size();
    socket_operations::getsockname<SockLenType>(getImpl(), ep.Data(), &s, e);
    if (e != no_error) {
        return EndPoint();
    }
    ep.SetSize(s);
    return ep;
}

lw_network::EndPoint lw_network::BasicSocket::remoteEndPoint(lw_network::error_code &e) const {
    EndPoint ep;

    std::size_t s = ep.Size();
    socket_operations::getpeername<SockLenType>(getImpl(), ep.Data(), &s, e);
    if (e != no_error) {
        return EndPoint();
    }
    ep.SetSize(s);
    return ep;
}

bool lw_network::BasicSocket::isOpen() const { return s_ != invalid_socket; }

void lw_network::BasicSocket::nonBlocking(bool yes, error_code &ec) {
    socket_operations::nonblocking(getImpl(), yes, ec);
}
