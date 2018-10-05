// Modified from OpenSSL Wiki : https://wiki.openssl.org/index.php/Simple_TLS_Server

#include "SslSession.hpp"

#include <openssl/err.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>

namespace elevation {
namespace daemon {

SslSession::SslSession(SSL* ssl)
    : m_ssl(ssl)
{ }

SslSession::~SslSession()
{
    SSL_free(m_ssl);
}

void SslSession::acceptNext(Socket& socket) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    m_clientFd = ::accept(socket.getFd(), (struct sockaddr*)&addr, &len);
    if (m_clientFd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    SSL_set_fd(m_ssl, socket.getFd());

    if (SSL_accept(m_ssl) <= 0) {
        throwSslError_();
    }
}

void SslSession::throwSslError_() const {
    constexpr int ERROR_BUFFER_SIZE = 300;
    char buf[ERROR_BUFFER_SIZE];
    ERR_error_string_n(ERR_get_error(), buf, ERROR_BUFFER_SIZE);
    throw std::runtime_error(buf);
}

} // namespace daemon
} // namespace elevation
