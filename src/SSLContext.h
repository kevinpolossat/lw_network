//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#ifndef LW_TCP_SERVER_SSLCONTEXT_H
#define LW_TCP_SERVER_SSLCONTEXT_H


#include <openssl/ssl.h>
#include <array>
#include <memory>
#include <string>
#include "SSLInit.h"

namespace lw_network {
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
        PEM = SSL_FILETYPE_PEM,
        ASN1 = SSL_FILETYPE_ASN1
    };
    enum class Mode {
        verify_none = SSL_VERIFY_NONE,
        verify_peer = SSL_VERIFY_PEER,
        verify_fail_if_no_peer_cert = SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
        verify_client_once = SSL_VERIFY_CLIENT_ONCE
    };
public:
    SSLContext(Method m);

    SSLContext(SSLContext const &other);

    SSLContext(SSLContext &&other);

    SSLContext &operator=(SSLContext const &other);

    SSLContext &operator=(SSLContext &&other);

    void useCertificateFile(std::string const & file, FileFormat pem);

    void useCertificateChainFile(std::string const & file);

    void usePrivateKeyFile(std::string const & file, FileFormat pem);

    void loadVerifyFile(std::string const & file, std::string const & path = "");

    void setVerifyMode(Mode mode);

    void setVerifyDepth(std::uint32_t depth);

    SSL_CTX *getLowLevelContext();

private:
    std::shared_ptr<SSL_CTX> ctx_;
    SSLInit &sslInit_;
private:
    using SSLMethodBuilder = const SSL_METHOD *(*)();
    static std::array<SSLMethodBuilder, 12> const methodBuilder_;

    bool checkPrivateKey_();
};
}

#endif //LW_TCP_SERVER_SSLCONTEXT_H
