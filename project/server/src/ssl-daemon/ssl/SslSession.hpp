#ifndef SSL_SSL_SESSION_HPP
#define SSL_SSL_SESSION_HPP

#include <openssl/ssl.h>

#include "communication/Socket.hpp"

namespace elevation {
namespace daemon {

class SslSession {
public:
    explicit SslSession(SSL* ssl);
    virtual ~SslSession();

    void acceptNext(Socket& socket);

protected:
    void throwSslError_() const;

protected:
    SSL* const m_ssl;
    int m_clientFd;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_SESSION_HPP
