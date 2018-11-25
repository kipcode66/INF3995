#ifndef HTTPS_DESCRIPTIONS_BLACKLISTAPI
#define HTTPS_DESCRIPTIONS_BLACKLISTAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>
#include <common/database/templates/User.hpp>


class BlacklistApi {
public:
    BlacklistApi(Pistache::Rest::Description& desc, elevation::Logger&);

protected:
    void getSuperviseurListenoire_  (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);
    void postSuperviseurBloquer_    (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);
    void postSuperviseurDebloquer_  (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);

    std::string generateUser_  (const User_t&);
    bool        checkIfAdmin_  (const Pistache::Rest::Request&);

    elevation::Logger& m_logger;
};
#endif // !HTTPS_DESCRIPTIONS_BLACKLISTAPI

