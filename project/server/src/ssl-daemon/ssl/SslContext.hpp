#ifndef SSL_SSL_CONTEXT_HPP
#define SSL_SSL_CONTEXT_HPP

#include "SslSession.hpp"

#include <openssl/ssl.h>

#include <string>

namespace elevation {
namespace daemon {

class SslContext {
public:
    static SslContext& getInstance();

    SslSession makeSession() const;

protected:
    explicit SslContext(const std::string& certificatePath, const std::string& privateKeyPath);
    virtual ~SslContext();

    SSL_CTX* createContext_();
    void configureContext_(const std::string& certificatePath, const std::string& privateKeyPath);

protected:
    SSL_CTX* m_ctx;

    static SslContext c_instance;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_CONTEXT_HPP
