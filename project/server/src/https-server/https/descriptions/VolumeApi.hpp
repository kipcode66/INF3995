#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

using namespace Pistache;


class VolumeApi {
public:
    VolumeApi(Pistache::Rest::Description& desc);

private:
    void GET_volume_              (const Rest::Request&, Http::ResponseWriter);
    void POST_volumeAug_          (const Rest::Request&, Http::ResponseWriter);
    void POST_volumeDim_          (const Rest::Request&, Http::ResponseWriter);
    void POST_sourdineActiver_    (const Rest::Request&, Http::ResponseWriter);
    void POST_sourdineDesactiver_ (const Rest::Request&, Http::ResponseWriter);
};

