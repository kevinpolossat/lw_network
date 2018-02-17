//
// Created by Kévin POLOSSAT on 11/02/2018.
//

#include <exception>
#include <openssl/err.h>
#include "SSLSocket.h"

lw_network::SSLSocket::SSLSocket(
        lw_network::SSLContext const &ctx,
        lw_network::Socket s):
        Socket(s), ctx_(ctx), ssl_(nullptr), sslInit_(lw_network::SSLInit::instance()) {}

lw_network::SSLSocket::SSLSocket(
        lw_network::Socket s): Socket(s), ctx_(), ssl_(nullptr), sslInit_(lw_network::SSLInit::instance()) {}

lw_network::SSLSocket::SSLSocket(
        const lw_network::SSLSocket &other):
        Socket(other),
        ctx_(other.ctx_),
        ssl_(other.ssl_),
        sslInit_(lw_network::SSLInit::instance()) {}

lw_network::SSLSocket::SSLSocket(
        lw_network::SSLSocket &&other):
        Socket(std::move(other)),
        ctx_(std::move(other.ctx_)),
        ssl_(std::move(other.ssl_)),
        sslInit_(lw_network::SSLInit::instance()) {}

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
    ssl_ = std::shared_ptr<SSL>(SSL_new(ctx_.getLowLevelContext()), [](auto sess) {
        SSL_free(sess);
    });
    if (!ssl_) {
        e = ERR_get_error();
    }
    else {
        SSL_set_fd(ssl_.get(), this->getImpl());
    }
}

void lw_network::SSLSocket::close(lw_network::error_code &e) {
   e = SSL_shutdown(ssl_.get());
    if (e == -1) {
        e = ERR_get_error();
        return ;
    }
    ssl_.reset();
    Socket::close(e);
}

signed_size_type lw_network::SSLSocket::recv(lw_network::Buffer &buffer, int, lw_network::error_code &e) {
    return this->read(buffer, e);
}

signed_size_type lw_network::SSLSocket::send(lw_network::Buffer &buffer, int, lw_network::error_code &e) {
    return this->write(buffer, e);
}

void lw_network::SSLSocket::setContext(lw_network::SSLContext const &ctx) {
    ctx_ = ctx;
}

