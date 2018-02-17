//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_SOCKET_H
#define LW_TCP_SERVER_SOCKET_H

#include <memory>
#include <type_traits>
#include "Socket.h"
#include "Reactor.h"
#include "Buffer.h"
#include "Operation.h"
#include "SSLSocket.h"

namespace lw_network {

template<typename Sock = Socket>
class ReactiveSocketBase : public Sock {
    static_assert(std::is_base_of<Socket, Sock>::value, "Socket base should be a derived class of Socket");
public:
    explicit ReactiveSocketBase(Reactor &reactor) : Sock(), reactor_(reactor) { register_(); }

    ReactiveSocketBase(Reactor &reactor, Sock socket): Sock(socket), reactor_(reactor) { register_(); }

    ~ReactiveSocketBase() = default;

    ReactiveSocketBase(ReactiveSocketBase const &other) = default;

    ReactiveSocketBase(ReactiveSocketBase &&other) = default;

    ReactiveSocketBase &operator=(ReactiveSocketBase const &other) {
        Sock::operator=(other);
        return *this;
    }

    ReactiveSocketBase &operator=(ReactiveSocketBase &&other) {
        Sock::operator=(other);
        return *this;
    }

    void close() {
        reactor_.unregisterHandler(this->getImpl(), lw_network::Reactor::read);
        reactor_.unregisterHandler(this->getImpl(), lw_network::Reactor::write);
        error_code ec = no_error;
        Sock::close(ec);
    }

    void async_read_some(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

    void async_read(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

    void async_write_some(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

    void async_write(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

private:
    Reactor &reactor_;

private:
    void register_() {
        reactor_.registerHandler(this->getImpl(), lw_network::Reactor::read);
        reactor_.registerHandler(this->getImpl(), lw_network::Reactor::write);
    }
};

using ReactiveSocket = ReactiveSocketBase<>;
using SSLReactiveSocket = ReactiveSocketBase<SSLSocket>;
// TODO FACTORIZE
template<typename T>
class ReadOperation : public Operation {
public:
    ReadOperation(
            ReactiveSocketBase<T> &s,
            Buffer b,
            std::function<void(std::size_t nbyte, error_code ec)> completionHandler):
            s_(s), ec_(no_error), nbyte_(0), b_(std::move(b)), completionHandler_(std::move(completionHandler)) {}

    bool handle() {
        nbyte_ = s_.recv(b_, 0, ec_);
        return b_.exhausted();
    }

    void complete() {
        completionHandler_(nbyte_, ec_);
    }

private:
    ReactiveSocketBase<T> &s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};


template<typename T>
class WriteOperation : public Operation {
public:
    WriteOperation(
            lw_network::ReactiveSocketBase<T> &s,
            lw_network::Buffer b,
            std::function<void(size_t, lw_network::error_code)> completionHandler):
            s_(s), ec_(no_error), nbyte_(0), b_(std::move(b)), completionHandler_(std::move(completionHandler)) {}

    bool handle() {
        nbyte_ = s_.send(b_, 0, ec_);
        return b_.exhausted();
    }

    void complete() {
        completionHandler_(nbyte_, ec_);
    }


private:
    ReactiveSocketBase<T> &s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

template<typename T>
class ReadSomeOperation : public Operation {
public:
    ReadSomeOperation(
            lw_network::ReactiveSocketBase<T> &s,
            lw_network::Buffer b,
            std::function<void(size_t, lw_network::error_code)> completionHandler):
            s_(s), ec_(no_error), nbyte_(0), b_(std::move(b)), completionHandler_(std::move(completionHandler)) {

    }

    bool handle() {
        nbyte_ = s_.recv(b_, 0, ec_);
        return true;
    }

    void complete() {
        completionHandler_(nbyte_, ec_);
    }

private:
    ReactiveSocketBase<T> &s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

template<typename T>
class WriteSomeOperation : public Operation {
public:
    WriteSomeOperation(
            lw_network::ReactiveSocketBase<T> &s,
            lw_network::Buffer b,
            std::function<void(size_t, lw_network::error_code)> completionHandler):
            s_(s), ec_(no_error), nbyte_(0), b_(std::move(b)), completionHandler_(std::move(completionHandler)) {}

    bool handle() {
        nbyte_ = s_.send(b_, 0, ec_);
        return true;
    }

    void complete() {
        completionHandler_(nbyte_, ec_);
    }


private:
    ReactiveSocketBase<T> &s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

}

#include "ReactiveSocketBase.icc"

#endif //LW_TCP_SERVER_SOCKET_H
