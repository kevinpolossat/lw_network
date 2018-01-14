//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_SOCKET_H
#define LW_TCP_SERVER_SOCKET_H

#include <memory>
#include "BasicSocket.h"
#include "Reactor.h"
#include "Buffer.h"

namespace lw_network {
class Socket: public BasicSocket {
public:
    explicit Socket(Reactor & reactor);
    ~Socket() = default;
    Socket(Socket const & other) = default;
    Socket(Socket && other) = default;

    Socket & operator = (Socket const & other);
    Socket & operator = (Socket && other);

    void close();
    void async_read_some(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    void async_read(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

    void async_write_some(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    void async_write(Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);

private:
    Reactor & reactor_;
};

// TODO FACTORIZE
class ReadOperation: public Operation {
public:
    ReadOperation(Socket & s, Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    bool handle();
    void complete();
private:
    Socket & s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

class WriteOperation: public Operation {
public:
    WriteOperation(Socket & s, Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    bool handle();
    void complete();
private:
    Socket & s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

class ReadSomeOperation: public Operation {
public:
    ReadSomeOperation(Socket & s, Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    bool handle();
    void complete();
private:
    Socket & s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};

class WriteSomeOperation: public Operation {
public:
    WriteSomeOperation(Socket & s, Buffer b, std::function<void(std::size_t nbyte, error_code ec)> completionHandler);
    bool handle();
    void complete();
private:
    Socket & s_;
    error_code ec_;
    std::size_t nbyte_;
    Buffer b_;
    std::function<void(std::size_t nbyte, error_code ec)> completionHandler_;
};
}

#endif //LW_TCP_SERVER_SOCKET_H
