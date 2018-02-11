//
// Created by Kévin POLOSSAT on 11/02/2018.
//

#include <exception>
#include <openssl/err.h>
#include "SSLSocket.h"

lw_network::SSLSocket::SSLSocket(lw_network::SSLContext &ctx, socket_type s): Socket(s), ctx_(ctx), ssl_(nullptr) {}

lw_network::SSLSocket::SSLSocket(const lw_network::SSLSocket &other): Socket(other), ctx_(other.ctx_), ssl_(nullptr) {}

lw_network::SSLSocket::SSLSocket(lw_network::SSLSocket &&other): Socket(other), ctx_(other.ctx_), ssl_(nullptr) {}

lw_network::SSLSocket &lw_network::SSLSocket::operator=(const lw_network::SSLSocket &other) {
    Socket::operator=(other);
    ctx_ = other.ctx_;
    ssl_ = other.ssl_;
    return *this;
}

lw_network::SSLSocket &lw_network::SSLSocket::operator=(lw_network::SSLSocket &&other) {
    Socket::operator=(other);
    ctx_ = std::move(other.ctx_);
    ssl_ = std::move(other.ssl_);
    return *this;
}

void lw_network::SSLSocket::connect(const lw_network::EndPoint &endPoint, lw_network::error_code &e) {
    Socket::connect(endPoint, e);
    if (e == lw_network::no_error) {
        initSession(e);
    }
}

void lw_network::SSLSocket::accept(lw_network::Socket &socket, lw_network::error_code &e) {
    Socket::accept(socket, e);
    if (e == lw_network::no_error) {
        initSession(e);
    }
}

signed_size_type lw_network::SSLSocket::read(lw_network::Buffer &buffer, lw_network::error_code &e) {
    auto ret = SSL_read(ssl_.get(), buffer.Data(), buffer.Size());
    if (ret == -1) {
        e = ERR_get_error();
    }
    else {
        buffer += ret;
    }
    return ret;
}

signed_size_type lw_network::SSLSocket::write(lw_network::Buffer &buffer, lw_network::error_code &e) {
    auto ret = SSL_write(ssl_.get(), buffer.Data(), buffer.Size());
    if (ret == -1) {
        e = ERR_get_error();
    }
    else {
        buffer += ret;
    }
    return ret;
}

void lw_network::SSLSocket::handShake(lw_network::SSLSocket::HandShakeType hst, lw_network::error_code &e) {
    if (hst == lw_network::SSLSocket::HandShakeType::server) {
        this->serverHandShake(e);
    }
    else {
        this->clientHandShake(e);
    }
}

void lw_network::SSLSocket::clientHandShake(lw_network::error_code &e) {
    e = SSL_connect(ssl_.get());
}

void lw_network::SSLSocket::serverHandShake(lw_network::error_code &e) {
    e = SSL_accept(ssl_.get());
}

void lw_network::SSLSocket::initSession(error_code &e) {
    ssl_ = std::shared_ptr<SSL>(SSL_new(ctx_), [](auto sess) {
        SSL_free(sess);
    });
    if (!ssl_) {
        e = ERR_get_error();
    }
    else {
        SSL_set_fd(ssl_.get(), this->getImpl());
    }
}
