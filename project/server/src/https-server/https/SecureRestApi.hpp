#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "http-server/http/RestApi.hpp"

#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Address);
    void init();
private:
    void createSecureDescription_();
    void getSuperviseurFile_(const Rest::Request&, Http::ResponseWriter);

private:
    Logger& m_logger;
};

} // namespace elevation

#endif // HTTPS_RESTAPI_HPP
