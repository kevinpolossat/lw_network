//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include <unistd.h>
#include <cerrno>
#include <type_traits>
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
    domain_ = domain;
    type_ = type;
    protocol_ = protocol;
    Open();
}

void lw_tcp_server::Socket::Open() {
    sock_fd_ = socket(domain_, type_, protocol_);
    if (sock_fd_ == -1) {
        throw OpenError(std::strerror(errno));
    }
}

void lw_tcp_server::Socket::Close() {
    domain_ = -1;
    type_ = -1;
    protocol_ = -1;
    int ret = close(sock_fd_);
    sock_fd_ = -1;
    if (ret == -1) {
        throw CloseError(std::strerror(errno));
    }
}

void lw_tcp_server::Socket::Listen(int nb_connection_allowed) {
    int ret = listen(sock_fd_, nb_connection_allowed);
    if (ret == -1) {
        throw ListenError(std::strerror(errno));
    }
}

template<typename T>
void lw_tcp_server::Socket::Bind(const T &addr) {
    static_assert(
            std::is_same<T, IPV4_ADDR>::value ||
            std::is_same<T, IPV6_ADDR>::value,
            "addr must be of type IPV4_ADDR or IPV6_ADDR");
    int ret = bind(sock_fd_, reinterpret_cast<sockaddr const *>(&addr), sizeof addr);
    if (ret == -1) {
        throw BindError(std::strerror(errno));
    }
}

template<typename T>
void lw_tcp_server::Socket::Connect(const T &addr) {
    static_assert(
            std::is_same<T, IPV4_ADDR>::value ||
            std::is_same<T, IPV6_ADDR>::value,
            "addr must be of type IPV4_ADDR or IPV6_ADDR");
    int ret = connect(sock_fd_, reinterpret_cast<sockaddr const *>(&addr), sizeof addr);
    if (ret == -1) {
        throw ConnectError(std::strerror(errno));
    }
}

template<typename T>
void lw_tcp_server::Socket::SetOpt(int optname, const T &value, int level) {
    int ret = setsockopt(sock_fd_, level, optname, &value, sizeof value);
    if (ret == -1) {
        throw SetOptError(std::strerror(errno));
    }
}

template<typename T>
void lw_tcp_server::Socket::GetOpt(int optname, T &res, socklen_t &sizeOfRes, int level) {
    sizeOfRes = sizeof res;
    int ret = getsockopt(sock_fd_, level, optname, &res, &sizeOfRes);
    if (ret == -1) {
        throw GetOptError(std::strerror(errno));
    }
}

void lw_tcp_server::Socket::Accept(lw_tcp_server::Socket &out) {
    sockaddr_storage their_addr;
    socklen_t addr_size;

    int ret = accept(sock_fd_, reinterpret_cast<sockaddr *>(&their_addr), &addr_size);
    if (ret == -1) {
        throw AcceptError(std::strerror(errno));
    }
    out.sock_fd_ = ret;
}

std::int64_t lw_tcp_server::Socket::Recv(char *buff, std::size_t len) {
    return recv(sock_fd_, buff, len, 0);
}

std::int64_t lw_tcp_server::Socket::Send(char *buff, std::size_t len) {
    return send(sock_fd_, buff, len, 0);
}
