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
        ~Socket();
        Socket(Socket const & other);
        Socket(Socket && other);
        Socket & operator = (Socket const & other);
        Socket & operator = (Socket && other);

        int Open();
        int Close();
        int Bind();
        int Connect();
        int Accept();
    private:
        int sock_fd_;
        int sock_opt_;
        std::string port_;
        struct addrinfo addr_;
    };
}


#endif //LW_NETWORK_SOCKET_H
