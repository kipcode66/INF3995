#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

using namespace Pistache;

class BlacklistApi {
public:
    BlacklistApi(Pistache::Rest::Description& desc);
private:
    void postSuperviseurBloquer_(const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurDebloquer_(const Rest::Request&, Http::ResponseWriter);
    void getSuperviseurListenoire_(const Rest::Request&, Http::ResponseWriter);
};

