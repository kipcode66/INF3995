#ifndef SSL_SSL_SESSION_HPP
#define SSL_SSL_SESSION_HPP

#include <openssl/ssl.h>

namespace elevation {
namespace daemon {

class SslSession {
public:
    explicit SslSession(SSL* ssl);
    virtual ~SslSession();

    void acceptNext();

protected:
    SSL* const m_ssl;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_SESSION_HPP
