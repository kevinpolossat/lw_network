//
// Created by Kévin POLOSSAT on 19/11/2017.
//

#ifndef LW_TCP_SERVER_SOCKETACCEPTOR_H
#define LW_TCP_SERVER_SOCKETACCEPTOR_H

#include <functional>
#include <memory>
#include "socket_operations.h"
#include "ReactiveSocketBase.h"
#include "Reactor.h"
#include "Operation.h"

namespace lw_network {

template<typename T = Socket>
class AcceptorBase: public T {
    static_assert(std::is_base_of<Socket, T>::value, "Socket base should be a derived class of Socket");
public:
    AcceptorBase(AcceptorBase const & other) = delete;
    AcceptorBase(AcceptorBase && other) = delete;
    AcceptorBase & operator = (AcceptorBase const & other) = delete;
    AcceptorBase & operator = (AcceptorBase && other) = delete;

    explicit AcceptorBase(lw_network::Reactor &reactor): Socket(), reactor_(reactor) {
        reactor_.registerHandler(this->getImpl(), lw_network::Reactor::OperationType::read);
    }

    ~AcceptorBase() {
        reactor_.unregisterHandler(this->getImpl(), Reactor::OperationType::read);
    }

    void asyncAccept(std::function<void(lw_network::ReactiveSocketBase<T>, error_code)> completionHandler);

    lw_network::Reactor &reactor() {
        return reactor_;
    }

private:
    Reactor &reactor_;
};

using Acceptor = AcceptorBase<>;
using SSLAcceptor = AcceptorBase<SSLSocket>;

template<typename T>
class AcceptOperation: public Operation {
public:
    AcceptOperation(
            std::function<void(lw_network::ReactiveSocketBase<T>, lw_network::error_code)> completionHandler,
            lw_network::AcceptorBase<T> &acceptor):
            completionHandler_(std::move(completionHandler)),
            acceptor_(acceptor),
            s_(acceptor.reactor()),
            ec_(no_error) {}

    void complete() {
        completionHandler_(s_, ec_);
    }

    bool handle() {
        acceptor_.accept(s_, ec_);
        return true;
    }

private:
    std::function<void(ReactiveSocketBase<T>, error_code)> completionHandler_;
    AcceptorBase<T> & acceptor_;
    ReactiveSocketBase<T> s_;
    error_code ec_;
};
}

#include "AcceptorBase.icc"

#endif //LW_TCP_SERVER_SOCKETACCEPTOR_H
