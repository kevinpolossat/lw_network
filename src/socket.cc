//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include "socket.h"

lw_network::Socket::Socket(): domain_(-1), type_(-1), protocol_(-1) {}

lw_network::Socket::Socket(int domain, int type, int protocol): domain_(domain), type_(type), protocol_(protocol) {}

lw_network::Socket::~Socket() {

}

lw_network::Socket::Socket(Socket &&other) noexcept {

}

lw_network::Socket &lw_network::Socket::operator=(Socket &&other) noexcept {
    return *this;
}

int lw_network::Socket::Domain() const {
    return domain_;
}

int lw_network::Socket::Type() const {
    return type_;
}

int lw_network::Socket::Protocol() const {
    return protocol_;
}
