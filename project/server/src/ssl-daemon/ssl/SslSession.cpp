// Modified from OpenSSL Wiki : https://wiki.openssl.org/index.php/Simple_TLS_Server

#include "SslSession.hpp"

#include <openssl/err.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>
#include <string>

namespace elevation {
namespace daemon {

SslSession::SslSession(SSL* ssl)
    : m_ssl(ssl)
    , m_clientSocket(nullptr)
{ }

SslSession::SslSession(SslSession&& that)
    : m_clientSocket(std::move(that.m_clientSocket))
{
    m_ssl = that.m_ssl;
    that.m_ssl = nullptr;
}

SslSession::~SslSession()
{
    if (m_ssl != nullptr) {
        SSL_free(m_ssl);
    }
}

SslSession& SslSession::operator=(SslSession&& that) {
    m_ssl = that.m_ssl;
    m_clientSocket = std::move(that.m_clientSocket);
    that.m_ssl = nullptr;
}

void SslSession::bindTo(std::unique_ptr<IpSocket> socket) {
    m_clientSocket = std::move(socket);
    SSL_set_fd(m_ssl, m_clientSocket->m_fd);

    int acceptStatus = SSL_accept(m_ssl);
    if (acceptStatus <= 0) {
        throw std::runtime_error("Failed SSL_accept");
    }
}

void SslSession::write(const std::string& data) {
    SSL_write(m_ssl, data.c_str(), data.size());
}

std::string SslSession::read() {
    char buffer[BUFFER_READ_SIZE];
    SSL_read(m_ssl, buffer, BUFFER_READ_SIZE);
    return std::string(buffer);
}

} // namespace daemon
} // namespace elevation
