//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_CONNECTOR_H
#define LW_TCP_SERVER_CONNECTOR_H

#include "Socket.h"
#include "Resolver.h"

namespace lw_network {
class Connector: public Socket {
public:
    Connector(lw_network::Reactor &re);
    ~Connector() = default;

    void connect(Resolver const & resolver);
    void asyncConnect(Resolver const & resolver, std::function<void(lw_network::error_code, EndPoint ep)>);
};
}

#endif //LW_TCP_SERVER_CONNECTOR_H
