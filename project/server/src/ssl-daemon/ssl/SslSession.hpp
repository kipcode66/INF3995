#ifndef SSL_SSL_SESSION_HPP
#define SSL_SSL_SESSION_HPP

#include <openssl/ssl.h>
#include <memory>

#include <common/os/Socket.hpp>

namespace elevation {
namespace daemon {

class SslSession {
public:
    static constexpr const uint32_t s_BUFFER_READ_SIZE = 16 << 10; ///< Size of the buffer used to read from the client.
                                                                   ///< SSLv3 requires this to be at most 16kB (see manpage of SSL_read).

public:
    explicit SslSession(SSL* ssl);
    SslSession(SslSession&& that);
    SslSession(const SslSession&) = delete;

    virtual ~SslSession();

    SslSession& operator=(SslSession&&);
    SslSession& operator=(const SslSession&) = delete;

    void bindTo(std::unique_ptr<Socket> socket);

    void write(const std::string& data);
    std::string read();

protected:
    void handleSslErrorsIfAny_(int sslReturnCode);

protected:
    SSL* m_ssl;
    std::unique_ptr<Socket> m_clientSocket;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_SESSION_HPP
