#ifndef HTTPS_DESCRIPTIONS_VOLUMEAPI
#define HTTPS_DESCRIPTIONS_VOLUMEAPI

#include <pistache/description.h>


class VolumeApi {
public:
    VolumeApi(Pistache::Rest::Description& desc);

private:
    void GET_volume_              (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_volumeAug_          (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_volumeDim_          (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_sourdineActiver_    (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_sourdineDesactiver_ (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
};
#endif // !HTTPS_DESCRIPTIONS_VOLUMEAPI

