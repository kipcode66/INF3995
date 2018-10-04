#include "SslSession.hpp"

namespace elevation {
namespace daemon {

SslSession::SslSession(SSL* ssl)
    : m_ssl(ssl)
{

}

SslSession::~SslSession()
{
    SSL_free(m_ssl);
}

} // namespace daemon
} // namespace elevation
