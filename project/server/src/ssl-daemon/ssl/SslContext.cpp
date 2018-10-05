#include "SslContext.hpp"

#include <openssl/err.h>

#include <elevation/daemon/Config.hpp>

namespace elevation {
namespace daemon {

SslContext SslContext::c_instance = SslContext(SSL_DAEMON_CERTIFICATE, SSL_DAEMON_PRIVATE_KEYFILE);

SslContext::SslContext(const std::string& certificatePath, const std::string& privateKeyPath)
    : m_ctx(nullptr)
{
    m_ctx = createContext_();
    configureContext_(certificatePath, privateKeyPath);
}

SslContext::~SslContext()
{
    if (m_ctx != nullptr) {
        SSL_CTX_free(m_ctx);
    }
}

SSL_CTX* SslContext::createContext_() {
    const SSL_METHOD* method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
    }

    return ctx;
}

void SslContext::configureContext_(const std::string& certificatePath, const std::string& privateKeyPath) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    if (SSL_CTX_use_certificate_file(m_ctx, certificatePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
    }

    if (SSL_CTX_use_PrivateKey_file(m_ctx, privateKeyPath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
    }
}

SslSession SslContext::makeSession() const {
    SSL* ssl = SSL_new(m_ctx);
    return SslSession(ssl);
}

} // namespace daemon
} // namespace elevation
