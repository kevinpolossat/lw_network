//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include <unistd.h>
#include <cerrno>
#include "socket.h"
#include "lw_tcp_server_exception.h"

lw_tcp_server::Socket::Socket(): domain_(-1), type_(-1), protocol_(-1), sock_fd_(-1) {}

lw_tcp_server::Socket::Socket(int domain, int type, int protocol):
        domain_(domain), type_(type), protocol_(protocol), sock_fd_(-1) {}

lw_tcp_server::Socket::~Socket() {
    if (sock_fd_!= -1) {
        Close();
    }
}

lw_tcp_server::Socket::Socket(Socket &&other) noexcept:
        domain_(other.domain_), type_(other.type_), protocol_(other.protocol_), sock_fd_(other.sock_fd_) {
    other.sock_fd_ = -1;
}

lw_tcp_server::Socket &lw_tcp_server::Socket::operator=(Socket &&other) noexcept {
    domain_ = other.domain_;
    type_ = other.type_;
    protocol_ = other.protocol_;
    sock_fd_ = other.sock_fd_;
    other.sock_fd_ = -1;
    return *this;
}

int lw_tcp_server::Socket::Domain() const {
    return domain_;
}

int lw_tcp_server::Socket::Type() const {
    return type_;
}

int lw_tcp_server::Socket::Protocol() const {
    return protocol_;
}

void lw_tcp_server::Socket::Open(int domain, int type, int protocol) {
    Close();
    domain_ = domain;
    type_ = type;
    protocol_ = protocol;
    Open();
}

void lw_tcp_server::Socket::Open() {
    sock_fd_ = socket(domain_, type_, protocol_);
    if (sock_fd_ == -1) {
        throw OpenError(strerror(errno));
    }
}

void lw_tcp_server::Socket::Close() {
    domain_ = -1;
    type_ = -1;
    protocol_ = -1;
    int ret = close(sock_fd_);
    sock_fd_ = -1;
    if (ret == -1) {
        throw CloseError(strerror(errno));
    }
}
