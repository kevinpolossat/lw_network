//
// Created by Kévin POLOSSAT on 11/02/2018.
//

#ifndef LW_TCP_SERVER_SSLSOCKET_H
#define LW_TCP_SERVER_SSLSOCKET_H

#include <memory>
#include <openssl/ssl.h>
#include "Socket.h"
#include "SSLContext.h"

namespace lw_network {
class SSLSocket: public Socket {
public:
    enum class HandShakeType {
        client,
        server
    };
public:
    explicit SSLSocket(lw_network::SSLContext & ctx, socket_type s = invalid_socket);
    ~SSLSocket() = default;

    SSLSocket(SSLSocket const & other);
    SSLSocket(SSLSocket && other);
    SSLSocket & operator = (SSLSocket const & other);
    SSLSocket & operator = (SSLSocket && other);

    void connect(EndPoint const & endPoint, error_code &e);
    void accept(Socket & socket, error_code &e);

    void handShake(HandShakeType hst, error_code &e);
    void clientHandShake(error_code &e);
    void serverHandShake(error_code &e);
    signed_size_type read(Buffer & buffer, error_code &e);
    signed_size_type write(Buffer & buffer, error_code &e);
    void initSession(error_code &e);
private:
    std::shared_ptr<SSL> ssl_;
    lw_network::SSLContext ctx_;
};
}


#endif //LW_TCP_SERVER_SSLSOCKET_H
