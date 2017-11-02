//
// Created by Kévin POLOSSAT on 01/11/2017.
//

#include "lw_tcp_server_error.h"
#include "resolver.h"

lw_tcp_server::Resolver::Resolver():
        family_(0), sock_type_(0), protocol_(0), flags_(0), addrs_(nullptr, [](addrinfo *to_free){ freeaddrinfo(to_free); }) {

}

lw_tcp_server::Resolver::Resolver(
        std::string const &node,
        std::string const &service,
        int family,
        int sock_type,
        int protocol,
        int flags):
        node_(node),
        service_(service),
        family_(family),
        sock_type_(sock_type),
        protocol_(protocol),
        flags_(flags),
        addrs_(nullptr, [](addrinfo *to_free){ freeaddrinfo(to_free); }) {}

lw_tcp_server::Resolver::Resolver(Resolver &&other) noexcept:
        node_(std::move(other.node_)),
        service_(std::move(other.service_)),
        family_(other.family_),
        sock_type_(other.sock_type_),
        protocol_(other.protocol_),
        flags_(other.flags_),
        addrs_(std::move(other.addrs_)) {
}

lw_tcp_server::Resolver &lw_tcp_server::Resolver::operator=(Resolver &&other) noexcept {
    node_ = std::move(other.node_);
    service_ = std::move(other.service_);
    family_ = other.family_;
    sock_type_ = other.sock_type_;
    protocol_ = other.protocol_;
    flags_ = other.flags_;
    addrs_.reset();
    addrs_ = std::move(other.addrs_);
    other.family_ = -1;
    other.sock_type_ = -1;
    other.protocol_ = -1;
    other.flags_ = -1;
    return *this;
}

std::string const &lw_tcp_server::Resolver::Node() const {
    return node_;
}

void lw_tcp_server::Resolver::SetNode(std::string const &node) {
    node_ = node;
}

std::string const &lw_tcp_server::Resolver::Service() const {
    return service_;
}

void lw_tcp_server::Resolver::SetService(std::string const &service) {
    service_ = service;
}

int lw_tcp_server::Resolver::Family() const {
    return family_;
}

void lw_tcp_server::Resolver::SetFamily(int family) {
    family_ = family;
}

int lw_tcp_server::Resolver::SockType() const {
    return sock_type_;
}

void lw_tcp_server::Resolver::SetSockType(int sock_type) {
    sock_type_ = sock_type;
}

int lw_tcp_server::Resolver::Protocol() const {
    return protocol_;
}

void lw_tcp_server::Resolver::SetProtocol(int protocol) {
    protocol_ = protocol;
}

int lw_tcp_server::Resolver::Flags() const {
    return flags_;
}

void lw_tcp_server::Resolver::SetFlags(int flags) {
    flags_ = flags;
}

lw_tcp_server::iterator lw_tcp_server::Resolver::Resolve(
        std::string const &node,
        std::string const &service,
        int family,
        int sock_type,
        int protocol,
        int flags) {
    node_ = node;
    service_ = service;
    family_ = family;
    sock_type_ = sock_type;
    protocol_ = protocol;
    flags_ = flags;
    return Resolve();
}

lw_tcp_server::iterator lw_tcp_server::Resolver::Resolve() {
    addrinfo hints = {0};
    addrinfo * res = nullptr;
    int status;

    hints.ai_family = family_;
    hints.ai_socktype = sock_type_;
    hints.ai_protocol = protocol_;
    hints.ai_flags = flags_;
    if ((status = getaddrinfo(
            node_.empty() ? nullptr : node_.c_str(),
            service_.empty() ? nullptr : service_.c_str(),
            &hints,
            &res)) != 0) {
        throw ResolveError(gai_strerror(status));
    }
    addrs_.reset(res);
    return lw_tcp_server::iterator(addrs_.get());
}

