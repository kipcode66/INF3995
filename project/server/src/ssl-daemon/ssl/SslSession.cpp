// Modified from OpenSSL Wiki : https://wiki.openssl.org/index.php/Simple_TLS_Server

#include "SslSession.hpp"

#include <openssl/err.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>
#include <string>

#include "communication/exception/SocketClosedException.hpp"
#include "elevation/daemon/Config.hpp"

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
    return *this;
}

void SslSession::bindTo(std::unique_ptr<Socket> socket) {
    m_clientSocket = std::move(socket);
    SSL_set_fd(m_ssl, m_clientSocket->m_fd);

    int acceptStatus = SSL_accept(m_ssl);
    if (acceptStatus <= 0) {
        throw std::runtime_error("Failed SSL_accept");
    }
}

void SslSession::write(const std::string& data) {
    int returnCode = ::SSL_write(m_ssl, data.c_str(), data.size());
    handleSslErrorsIfAny_(returnCode);
}

std::string SslSession::read() {
    char buffer[s_BUFFER_READ_SIZE + 1];
    int readSize = ::SSL_read(m_ssl, buffer, s_BUFFER_READ_SIZE);
    handleSslErrorsIfAny_(readSize);
    buffer[readSize] = '\0';
    return std::string(buffer);
}

void SslSession::handleSslErrorsIfAny_(int sslReturnCode) {
    if (sslReturnCode <= 0) {
        int errorNumber = ::SSL_get_error(m_ssl, sslReturnCode);

        std::string failureReason;
        switch(errorNumber) {
            case SSL_ERROR_ZERO_RETURN: {
                throw SocketClosedException();
                break;
            }
            case SSL_ERROR_WANT_READ: {
                failureReason = "No data to read";
                break;
            }
            case SSL_ERROR_WANT_WRITE: {
                failureReason = "No data to write";
                break;
            }
            case SSL_ERROR_WANT_CONNECT: // fallthrough
            case SSL_ERROR_WANT_ACCEPT: {
                failureReason = "Connection not yet established";
                break;
            }
            case SSL_ERROR_SYSCALL: {
                if (::ERR_get_error() != 0) {
                    failureReason = std::string("SSL system call failure. This can be caused by a distrusted "
                                                "certificate, in which case add the \"") + SSL_DAEMON_ISSUER_CERTIFICATE +
                                                "\" file as a trusted authority.";
                }
                else {
                    throw SocketClosedException();
                }
            }
            case SSL_ERROR_SSL: {
                failureReason = std::string("SSL messed up : ") + ::ERR_error_string(::ERR_get_error(), NULL);
                break;
            }
            default: {
                failureReason = "Non-trivial";
                break;
            }
        }

        throw std::runtime_error("SSL read/write error : " + failureReason);
    }
}

} // namespace daemon
} // namespace elevation
