#ifndef SSL_SSL_SESSION_HPP
#define SSL_SSL_SESSION_HPP

#include <openssl/ssl.h>

#include "communication/Socket.hpp"

namespace elevation {
namespace daemon {

class SslSession {
public:
    static constexpr int NO_FD = -1;

public:
    explicit SslSession(SSL* ssl);
    SslSession(SslSession&& that);
    SslSession(const SslSession&) = delete;

    virtual ~SslSession();

    SslSession& operator=(SslSession&&);
    SslSession& operator=(const SslSession&) = delete;

    void bindTo(Socket& socket);

protected:
    SSL* m_ssl;
    int m_clientFd;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_SESSION_HPP
