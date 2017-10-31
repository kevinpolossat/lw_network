//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#ifndef LW_NETWORK_SOCKET_H
#define LW_NETWORK_SOCKET_H


#include <string>
#include <netdb.h>

namespace lw_network {
    class Socket {
    public:
        Socket();
        Socket(int domain, int type, int protocol);
        ~Socket();
        Socket(Socket && other) noexcept;
        Socket & operator = (Socket && other) noexcept;

        Socket(Socket const & other) = delete;
        Socket & operator = (Socket const & other) = delete;

        int Domain() const;
        int Type() const;
        int Protocol() const;
    private:
        int domain_;
        int type_;
        int protocol_;
    };
}


#endif //LW_NETWORK_SOCKET_H
