//
// Created by Kévin POLOSSAT on 08/01/2018.
//

#ifndef LW_TCP_SERVER_BASICSOCKET_H
#define LW_TCP_SERVER_BASICSOCKET_H


#include <vector>
#include "SocketAcceptor.h"

namespace lw_network {
class BasicSocket {
public:
    BasicSocket();
    ~BasicSocket();

    error_code open(int protocol);
    error_code close();
    error_code bind(const struct sockaddr *addr);
    error_code listen(int backlog = 10/*TODO change default value ?*/);
    error_code connect(BasicSocket & s, struct sockaddr *addr); // TODO replace sockaddr by endpoint
    error_code accept(BasicSocket & s);
    error_code setOption();
    error_code getOption();
    signed_size_type recv(std::vector<unsigned char> & buffer, int flags, lw_network::error_code &e);
    signed_size_type send(std::vector<unsigned char> const & buffer, int flags, lw_network::error_code &e);
};
}


#endif //LW_TCP_SERVER_BASICSOCKET_H
