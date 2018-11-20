#ifndef HTTPS_DESCRIPTIONS_BLACKLISTAPI
#define HTTPS_DESCRIPTIONS_BLACKLISTAPI
#include <pistache/description.h>


class BlacklistApi {
public:
    BlacklistApi(Pistache::Rest::Description& desc);

private:
    void postSuperviseurBloquer_  (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void postSuperviseurDebloquer_(const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void getSuperviseurListenoire_(const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
};
#endif // !HTTPS_DESCRIPTIONS_BLACKLISTAPI

