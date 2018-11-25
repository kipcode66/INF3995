#ifndef HTTPS_DESCRIPTIONS_AUTHAPI
#define HTTPS_DESCRIPTIONS_AUTHAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>

class AuthApi {
public:
    AuthApi(Pistache::Rest::Description&, elevation::Logger&);

private:
    void superviseurLogin_   (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    void superviseurLogout_  (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    void postChangePassword_ (const Pistache::Rest::Request& request,
                              Pistache::Http::ResponseWriter response);
    elevation::Logger& m_logger;
};
#endif // !HTTPS_DESCRIPTIONS_AUTHAPI

