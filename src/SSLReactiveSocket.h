//
// Created by Kévin POLOSSAT on 12/02/2018.
//

#ifndef LW_TCP_SERVER_SSLREACTIVESOCKET_H
#define LW_TCP_SERVER_SSLREACTIVESOCKET_H

#include "ReactiveSocketBase.h"
#include "SSLSocket.h"

namespace lw_network {
class SSLReactiveSocket: public ReactiveSocketBase<SSLSocket> {
public:
    SSLReactiveSocket(
            Reactor & reactor,
            ReactiveSocketBase<SSLSocket> s,
            SSLContext & ctx): ReactiveSocketBase<SSLSocket>(reactor, SSLSocket(ctx, s.getImpl())) {}
    SSLReactiveSocket(SSLReactiveSocket const & other) = default;
    SSLReactiveSocket(SSLReactiveSocket && other) = default;
    SSLReactiveSocket &operator = (SSLReactiveSocket const & other) = default;
    SSLReactiveSocket &operator = (SSLReactiveSocket && other) = default;

    void asyncHandShake(lw_network::SSLSocket::HandShakeType hst);
};
}


#endif //LW_TCP_SERVER_SSLREACTIVESOCKET_H
