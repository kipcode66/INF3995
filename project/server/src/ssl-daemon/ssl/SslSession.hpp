#ifndef SSL_SSL_SESSION_HPP
#define SSL_SSL_SESSION_HPP

#include <openssl/ssl.h>
#include <memory>

#include "communication/IpSocket.hpp"

namespace elevation {
namespace daemon {

class SslSession {
public:
    explicit SslSession(SSL* ssl);
    SslSession(SslSession&& that);
    SslSession(const SslSession&) = delete;

    virtual ~SslSession();

    SslSession& operator=(SslSession&&);
    SslSession& operator=(const SslSession&) = delete;

    void bindTo(std::unique_ptr<IpSocket> socket);

protected:
    SSL* m_ssl;
    std::unique_ptr<IpSocket> m_clientSocket;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_SESSION_HPP
