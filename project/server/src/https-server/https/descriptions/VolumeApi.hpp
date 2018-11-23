#ifndef HTTPS_DESCRIPTIONS_VOLUMEAPI
#define HTTPS_DESCRIPTIONS_VOLUMEAPI

#include <pistache/description.h>

#include <common/logger/Logger.hpp>
#include <common/mp3/communication/Mp3EventClientSocket.hpp>

namespace elevation {

class VolumeApi {
public:
    VolumeApi(Pistache::Rest::Description& desc, Logger& logger, Mp3EventClientSocket playerEventSocket);

private:
    void GET_volume_              (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_volumeAssigner_     (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_sourdineActiver_    (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);
    void POST_sourdineDesactiver_ (const Pistache::Rest::Request&,
                                   Pistache::Http::ResponseWriter);

    bool isAdminAuthenticated_(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter& response);

private:
    Logger& m_logger;
    Mp3EventClientSocket m_playerEventSocket;
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_VOLUMEAPI

