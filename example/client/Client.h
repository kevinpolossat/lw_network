//
// Created by Kévin POLOSSAT on 15/01/2018.
//

#ifndef LW_TCP_SERVER_CLIENT_H
#define LW_TCP_SERVER_CLIENT_H


#include <string>
#include <array>
#include "Socket.h"

class Client {
public:
    void start(std::string const & host = "localhost", std::string const & port = "27015");
private:
    bool connect_(std::string const & host, std::string const & port);
    lw_network::Socket s_;
    std::array<char, 1024> bufferRead_;
    std::string bufferWrite_;
};


#endif //LW_TCP_SERVER_CLIENT_H
