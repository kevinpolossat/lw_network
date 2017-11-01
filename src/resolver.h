//
// Created by Kévin POLOSSAT on 01/11/2017.
//

#ifndef LW_TCP_SERVER_RESOLVER_H
#define LW_TCP_SERVER_RESOLVER_H

#include <string>
#include <netdb.h>
#include <functional>

namespace lw_tcp_server {
    template<typename T>
    class Iterator: public std::iterator<std::forward_iterator_tag, T, ptrdiff_t, T*, T&> {
    public:
        static_assert(
                std::is_same<T, addrinfo>::value ||
                std::is_same<T, addrinfo const>::value,
                "addr must be of type addrinfo or addrinfo const");

        explicit Iterator(T * ptr = nullptr) { m_ptr_ = ptr; }

        Iterator(Iterator<T> const & rawIterator) = default;
        Iterator(Iterator<T> && rawIterator) noexcept : m_ptr_(rawIterator.m_ptr_) { rawIterator.m_ptr_ = nullptr; }
        ~Iterator() = default;

        Iterator<T> & operator = (Iterator<T> const & rawIterator) = default;
        Iterator<T> & operator = (Iterator<T> && rawIterator) noexcept {
            m_ptr_ = rawIterator.m_ptr_;
            rawIterator.m_ptr_ = nullptr;
        }
        Iterator<T> & operator = (T * ptr) {
            m_ptr_ = ptr;
            return *this;
        }

        bool operator == (Iterator<T> const & rawIterator) const {
            return m_ptr_ == rawIterator.GetConstPtr();
        }
        bool operator != (Iterator<T> const & rawIterator) const {
            return m_ptr_ != rawIterator.GetConstPtr();
        }

        Iterator<T> & operator ++ () {
            m_ptr_ = m_ptr_->ai_next;
            return (*this);
        }
        Iterator<T> operator ++ (int) {
            auto temp(*this);
            m_ptr_ = m_ptr_->ai_next;
            return temp;
        }

        friend void swap(Iterator<T> & lhs, Iterator<T> & rhs) {
            auto * s = lhs.m_ptr_;
            lhs.m_ptr_ = rhs.m_ptr_;
            rhs.m_ptr_ = s;
        }

        T & operator * () { return *m_ptr_; }
        T const & operator * () const {return *m_ptr_; }
        T * operator -> () { return m_ptr_; }

        T * getPtr() const { return m_ptr_; }
        T const * GetConstPtr() const { return m_ptr_; }

    protected:
        T * m_ptr_;
    };

    using iterator = Iterator<addrinfo>;
    using const_iterator = Iterator<const addrinfo>;

    class Resolver {
    public:
        explicit Resolver();
        Resolver(
                std::string const &node,
                std::string const & service,
                int family,
                int sock_type,
                int protocol,
                int flags);
        Resolver(Resolver const & other) = delete;
        Resolver(Resolver && other) noexcept;
        ~Resolver() = default;
        Resolver & operator = (Resolver const & other) = delete;
        Resolver & operator = (Resolver && other) noexcept;

        std::string const & Node() const;
        void SetNode(std::string const & node);

        std::string const & Service() const;
        void SetService(std::string const & service);

        int Family() const;
        void SetFamily(int family);

        int SockType() const;
        void SetSockType(int sock_type);

        int Protocol() const;
        void SetProtocol(int protocol);

        int Flags() const;
        void SetFlags(int flags);

        lw_tcp_server::iterator Resolve(
                std::string const & node,
                std::string const & service,
                int family,
                int sock_type,
                int protocol,
                int flags);

        lw_tcp_server::iterator Resolve();

    private:
        std::string node_;
        std::string service_;
        int family_;
        int sock_type_;
        int protocol_;
        int flags_;
        std::unique_ptr<addrinfo, std::function<void(addrinfo*)>> addrs_;

    };
}


#endif //LW_TCP_SERVER_RESOLVER_H
