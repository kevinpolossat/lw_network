//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#ifndef LW_TCP_SERVER_SSLCONTEXT_H
#define LW_TCP_SERVER_SSLCONTEXT_H


#include <openssl/ssl.h>
#include <array>
#include <memory>
#include <string_view>

class SSLContext {
public:
    enum class Method {
        SSLv2,
        SSLv2_server,
        SSLv2_client,
        SSLv3,
        SSLv3_server,
        SSLv3_client,
        TLSv1,
        TLSv1_server,
        TLSv1_client,
        SSLv23,
        SSLv23_server,
        SSLv23_client
    };
    enum class FileFormat {
        PEM,
        ASN1
    };

public:
    SSLContext(Method m) throw(std::runtime_error);
    SSLContext(SSLContext const & other) = delete;
    SSLContext(SSLContext && other) = delete;
    SSLContext & operator = (SSLContext const & other) = delete;
    SSLContext & operator = (SSLContext && other) = delete;

    void useCertificateFile(std::string_view file, FileFormat pem) throw(std::runtime_error);
    void useCertificateChainFile(std::string_view file) throw(std::runtime_error);
    void usePrivateKeyFile(std::string_view file, FileFormat pem) throw(std::runtime_error);
private:
    std::shared_ptr<SSL_CTX> ctx_;
private:
    using SSLMethodBuilder = const SSL_METHOD * (*)();
    static std::array<SSLMethodBuilder, 12> const methodBuilder_;
    bool checkPrivateKey_();
};


#endif //LW_TCP_SERVER_SSLCONTEXT_H
