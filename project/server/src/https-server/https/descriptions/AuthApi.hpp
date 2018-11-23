#ifndef HTTPS_DESCRIPTIONS_AUTHAPI
#define HTTPS_DESCRIPTIONS_AUTHAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>

namespace elevation {

class AuthApi {
public:
    AuthApi(Pistache::Rest::Description&, Logger&);

private:
    void superviseurLogin_   (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    void superviseurLogout_  (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    void postChangePassword_ (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    Logger& m_logger;
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_AUTHAPI

