//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#ifndef LW_NETWORK_SOCKET_H
#define LW_NETWORK_SOCKET_H


#include <string>
#include <netdb.h>

namespace lw_tcp_server {
    class Socket {
    public:
        using IPV4_ADDR = struct sockaddr_in;
        using IPV6_ADDR = struct sockaddr_in6;

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

        void Open(int domain, int type, int protocol);
        void Open();
        void Close();

        template<typename T>
        void Bind(T const & addr);
        void Listen(int nb_connection_allowed = 10);

        template<typename T>
        void Connect(T const & addr);

        void SetOpt();
        void UnsetOpt();

    private:
        int domain_;
        int type_;
        int protocol_;
        int sock_fd_;
    };
}


#endif //LW_NETWORK_SOCKET_H
