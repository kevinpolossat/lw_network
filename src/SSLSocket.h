//
// Created by Kévin POLOSSAT on 11/02/2018.
//

#ifndef LW_TCP_SERVER_SSLSOCKET_H
#define LW_TCP_SERVER_SSLSOCKET_H

#include <memory>
#include <openssl/ssl.h>
#include "Socket.h"
#include "SSLContext.h"
#include "SSLInit.h"

namespace lw_network {
class SSLSocket: public Socket {
public:
    enum class HandShakeType {
        client,
        server
    };
public:
    explicit SSLSocket(lw_network::SSLContext const & ctx, Socket s = invalid_socket);
    explicit SSLSocket(Socket s = invalid_socket);
    ~SSLSocket() = default;

    SSLSocket(SSLSocket const & other);
    SSLSocket(SSLSocket && other);
    SSLSocket & operator = (SSLSocket const & other);
    SSLSocket & operator = (SSLSocket && other);

    void close(error_code &e) override;
    signed_size_type recv(Buffer & buffer, int flags, error_code &e) override;
    signed_size_type send(Buffer & buffer, int flags, error_code &e) override;

    void handShake(HandShakeType hst, error_code &e);
    void clientHandShake(error_code &e);
    void serverHandShake(error_code &e);
    signed_size_type read(Buffer & buffer, error_code &e);
    signed_size_type write(Buffer & buffer, error_code &e);
    void initSession(error_code &e);
    void setContext(lw_network::SSLContext const & ctx);
private:
    std::shared_ptr<SSL> ssl_;
    lw_network::SSLContext ctx_;
    lw_network::SSLInit & sslInit_;
};
}


#endif //LW_TCP_SERVER_SSLSOCKET_H
