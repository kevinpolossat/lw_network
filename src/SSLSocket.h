//
// Created by Kévin POLOSSAT on 11/02/2018.
//

#ifndef LW_TCP_SERVER_SSLSOCKET_H
#define LW_TCP_SERVER_SSLSOCKET_H

#include "socket_operations.h"

namespace lw_network {
class SSLSocket {
    SSLSocket(socket_type s = invalid_socket);
    ~SSLSocket();

    SSLSocket(SSLSocket const & other);
    SSLSocket(SSLSocket && other);
    SSLSocket & operator = (SSLSocket const & other);
    SSLSocket & operator = (socket_type s);
    SSLSocket & operator = (SSLSocket && other);
};
}


#endif //LW_TCP_SERVER_SSLSOCKET_H
