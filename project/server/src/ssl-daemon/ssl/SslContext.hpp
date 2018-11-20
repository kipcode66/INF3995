#ifndef SSL_SSL_CONTEXT_HPP
#define SSL_SSL_CONTEXT_HPP

#include "SslSession.hpp"

#include <openssl/ssl.h>

#include <string>
#include <memory>

#include <common/os/ListenerSocket.hpp>

namespace elevation {
namespace daemon {

class SslContext {
public:
    /**
     * @brief Creates and configures the singleton instance of SslContext.
     * @param[in] portNum The number of the port that will accept SSL connections.
     */
    static void createInstance(uint16_t portNum);
    static SslContext& getInstance();

public:
    SslSession acceptSession();

    virtual ~SslContext();

protected:
    explicit SslContext(ListenerSocket socket, const std::string& certificatePath, const std::string& privateKeyPath);

    void initOpenSsl_();
    SSL_CTX* createContext_();
    void configureContext_(const std::string& certificatePath, const std::string& privateKeyPath);

    void throwSslError_();

protected:
    SSL_CTX* m_ctx;
    ListenerSocket m_socket;

protected:
    static std::unique_ptr<SslContext> s_instance;
};

} // namespace daemon
} // namespace elevation

#endif // !SSL_SSL_CONTEXT_HPP
