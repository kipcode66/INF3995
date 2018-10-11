// Modified from OpenSSL Wiki : https://wiki.openssl.org/index.php/Simple_TLS_Server

#include "SslContext.hpp"

#include <cstring>
#include <stdexcept>

#include <openssl/err.h>

#include <elevation/daemon/Config.hpp>

namespace elevation {
namespace daemon {

std::unique_ptr<SslContext> SslContext::c_instance = nullptr;

void SslContext::createInstance(uint16_t portNum) {
    if (c_instance != nullptr) {
        ListenerSocket socket(portNum);
        c_instance = std::unique_ptr<SslContext>(
            new SslContext(std::move(socket), SSL_DAEMON_CERTIFICATE, SSL_DAEMON_PRIVATE_KEYFILE)
        );
    }
    else {
        throw std::logic_error("Cannot create SslContext instance : already exists.");
    }
}

SslContext& SslContext::getInstance() {
    if (c_instance != nullptr) {
        return *c_instance;
    }
    else {
        throw std::logic_error("Cannot get SslContext instance : not yet created.");
    }
}

SslContext::SslContext(ListenerSocket socket, const std::string& certificatePath, const std::string& privateKeyPath)
    : m_ctx(nullptr)
    , m_socket(std::move(socket))
{
    initOpenSsl_();
    m_ctx = createContext_();
    configureContext_(certificatePath, privateKeyPath);
}

SslContext::~SslContext()
{
    if (m_ctx != nullptr) {
        SSL_CTX_free(m_ctx);
    }
    EVP_cleanup();
}

void SslContext::initOpenSsl_() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

SSL_CTX* SslContext::createContext_() {
    const SSL_METHOD* method;
    SSL_CTX *ctx;

    method = SSLv23_server_method();

    ctx = SSL_CTX_new(method);
    if (!ctx) {
        throwSslError_();
    }

    return ctx;
}

void SslContext::configureContext_(const std::string& certificatePath, const std::string& privateKeyPath) {
    SSL_CTX_set_ecdh_auto(ctx, 1);

    if (SSL_CTX_use_certificate_file(m_ctx, certificatePath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throwSslError_();
    }

    if (SSL_CTX_use_PrivateKey_file(m_ctx, privateKeyPath.c_str(), SSL_FILETYPE_PEM) <= 0) {
        throwSslError_();
    }
}

void SslContext::throwSslError_() {
    constexpr int ERROR_BUFFER_SIZE = 300;
    char buf[ERROR_BUFFER_SIZE];
    ERR_error_string_n(ERR_get_error(), buf, ERROR_BUFFER_SIZE);
    throw std::runtime_error(buf);
}

SslSession SslContext::acceptSession() {
    std::unique_ptr<Socket> acceptedClient(new Socket(m_socket.accept()));
    SSL* ssl = SSL_new(m_ctx);
    SslSession session(ssl);
    session.bindTo(std::move(acceptedClient));
    return session;
}

} // namespace daemon
} // namespace elevation
