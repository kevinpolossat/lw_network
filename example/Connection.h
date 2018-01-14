//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_CONNECTION_H
#define LW_TCP_SERVER_CONNECTION_H

#include "Socket.h"

class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection> {
public:
    Connection(lw_network::Socket s, ConnectionManager & cm);
    Connection(Connection const & other) = delete;
    Connection & operator = (Connection const & other) = delete;
    void start();
    void stop();
private:
    void doWrite();
private:
    lw_network::Socket s_;
    ConnectionManager &cm_;
    std::string greeting_;
};


#endif //LW_TCP_SERVER_CONNECTION_H
