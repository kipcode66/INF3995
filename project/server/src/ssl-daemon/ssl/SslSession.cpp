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

SslSession::SslSession(SslSession&& that) {
    *this = std::move(that);
}

SslSession::~SslSession()
{
    if (m_ssl != nullptr) {
        SSL_free(m_ssl);
    }
    if (m_clientFd != NO_FD) {
        ::close(m_clientFd);
    }
}

SslSession& SslSession::operator=(SslSession&& that) {
    m_ssl = that.m_ssl;
    m_clientFd = that.m_clientFd;
    that.m_ssl = nullptr;
    that.m_clientFd = NO_FD;
}

void SslSession::acceptNext(Socket& socket) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    m_clientFd = ::accept(socket.getFd(), (struct sockaddr*)&addr, &len);
    if (m_clientFd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    SSL_set_fd(m_ssl, m_clientFd);

    int acceptStatus = SSL_accept(m_ssl);
    if (acceptStatus <= 0) {
        throw std::runtime_error("Failed SSL_accept");
    }
    else {
        const char reply[] = "test\n";
        SSL_write(m_ssl, reply, strlen(reply));
    }
}

} // namespace daemon
} // namespace elevation
