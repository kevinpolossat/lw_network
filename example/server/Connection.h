//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_CONNECTION_H
#define LW_TCP_SERVER_CONNECTION_H

#include <array>
#include <string>
#include "ReactiveSocket.h"

class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection> {
public:
    Connection(lw_network::ReactiveSocket s, ConnectionManager & cm);
    Connection(Connection const & other) = delete;
    Connection & operator = (Connection const & other) = delete;
    void start();
    void stop();
private:
    void doRead_();
    void doWrite_();
private:
    lw_network::ReactiveSocket s_;
    ConnectionManager &cm_;
    std::array<char, 1024> bufferRead_;
    std::string bufferWrite_;
};


#endif //LW_TCP_SERVER_CONNECTION_H
