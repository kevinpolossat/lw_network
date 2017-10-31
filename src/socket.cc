//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include <unistd.h>
#include "socket.h"

lw_tcp_socket::Socket::Socket(): domain_(-1), type_(-1), protocol_(-1) {}

lw_tcp_socket::Socket::Socket(int domain, int type, int protocol): domain_(domain), type_(type), protocol_(protocol) {}

lw_tcp_socket::Socket::~Socket() {

}

lw_tcp_socket::Socket::Socket(Socket &&other) noexcept {

}

lw_tcp_socket::Socket &lw_tcp_socket::Socket::operator=(Socket &&other) noexcept {
    return *this;
}

int lw_tcp_socket::Socket::Domain() const {
    return domain_;
}

int lw_tcp_socket::Socket::Type() const {
    return type_;
}

int lw_tcp_socket::Socket::Protocol() const {
    return protocol_;
}

void lw_tcp_socket::Socket::Open(int domain, int type, int protocol) {
    Close();
    domain_ = domain;
    type_ = type;
    protocol_ = protocol;
    Open();
}

void lw_tcp_socket::Socket::Open() {

}

void lw_tcp_socket::Socket::Close() {
    domain_ = -1;
    type_ = -1;
    protocol_ = -1;
}
