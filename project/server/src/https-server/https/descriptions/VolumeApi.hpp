#ifndef HTTPS_DESCRIPTIONS_VOLUMEAPI
#define HTTPS_DESCRIPTIONS_VOLUMEAPI

#include <pistache/description.h>

namespace elevation {

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

private:
    Mp3EventClientSocket m_socket;
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_VOLUMEAPI

