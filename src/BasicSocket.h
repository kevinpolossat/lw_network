//
// Created by Kévin POLOSSAT on 08/01/2018.
//

#ifndef LW_TCP_SERVER_BASICSOCKET_H
#define LW_TCP_SERVER_BASICSOCKET_H

#include "Buffer.h"
#include "Protocol.h"
#include "EndPoint.h"

namespace lw_network {
class BasicSocket {
#if defined (__linux__) || defined (__APPLE__)
    using SockLenType = socklen_t;
#elif defined (_WIN32) || defined (_WIN64)
    #error "TODO DEFINE WINDOWS"
#else
#error "unknown platform"
#endif
public:
    BasicSocket(socket_type s = invalid_socket);
    virtual ~BasicSocket();

    BasicSocket(BasicSocket const & other);
    BasicSocket(BasicSocket && other);
    BasicSocket & operator = (BasicSocket const & other);
    BasicSocket & operator = (socket_type s);
    BasicSocket & operator = (BasicSocket && other);

    void open(Protocol const & proto, error_code &e);
    bool isOpen() const;
    void close(error_code &e);
    void bind(EndPoint const & endpoint, error_code &e);
    void listen(int backlog, error_code &e);
    void connect(EndPoint const & endPoint, error_code &e);
    void accept(BasicSocket & socket, error_code &e);
    void setOption(int level, int optname, void const *optval, std::size_t optlen, error_code &e);
    void getOption(int level, int optname, void *optval, std::size_t *optlen, error_code &e) const;
    signed_size_type recv(Buffer & buffer, int flags, error_code &e);
    signed_size_type send(Buffer & buffer, int flags, error_code &e);
    EndPoint localEndPoint(error_code &e) const;
    EndPoint remoteEndPoint(error_code &e) const;
    void nonBlocking(bool yes, error_code &ec);
    socket_type getImpl() const;
    void setImpl(socket_type s);
private:
    socket_type s_;
};
}


#endif //LW_TCP_SERVER_BASICSOCKET_H
