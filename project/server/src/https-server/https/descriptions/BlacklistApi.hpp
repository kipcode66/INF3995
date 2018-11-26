#ifndef HTTPS_DESCRIPTIONS_BLACKLISTAPI
#define HTTPS_DESCRIPTIONS_BLACKLISTAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>
#include <common/database/templates/User.hpp>

namespace elevation {

class BlacklistApi {
public:
    BlacklistApi(Pistache::Rest::Description& desc, Logger&);

protected:
    void getSuperviseurListenoire_  (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);
    void postSuperviseurBloquer_    (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);
    void postSuperviseurDebloquer_  (const Pistache::Rest::Request&,
                                     Pistache::Http::ResponseWriter);

    bool checkIfAdmin_              (const Pistache::Rest::Request&);

    Logger& m_logger;
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_BLACKLISTAPI
