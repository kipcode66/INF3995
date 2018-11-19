#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "http-server/http/RestApi.hpp"
#include "descriptions/VolumeApi.hpp"
#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Address addr, Logger& logger, FileCache&);
    void init();

private:
    void createSecureDescription_          ();

    /* void createVolumeDescription_          (Rest::Description&); */
    void createBlacklistDescription_       (Rest::Description&);
    void createStatisticsDescription_      (Rest::Description&);
    void createAuthDescription_            (Rest::Description&);
    void createFileManagementDescription_  (Rest::Description&);

    VolumeApi         m_volumeApi;
    /* BlacklistApi      m_volumeApi; */
    /* StatisticsApi     m_volumeApi; */
    /* AuthApi           m_volumeApi; */
    /* FileManagementApi m_volumeApi; */

    void getSuperviseurFile_               (const Rest::Request&, Http::ResponseWriter);
    void deleteSuperviseurChanson_         (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurInversion_         (const Rest::Request&, Http::ResponseWriter);

    void getSuperviseurStatistiques_       (const Rest::Request&, Http::ResponseWriter);

    void postSuperviseurBloquer_           (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurDebloquer_         (const Rest::Request&, Http::ResponseWriter);
    void getSuperviseurListenoire_         (const Rest::Request&, Http::ResponseWriter);

    void postChangePassword_               (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogin_                 (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogout_                (const Rest::Request&, Http::ResponseWriter);

private:
    Logger& m_logger;
};

} // namespace elevation

#endif // !HTTPS_RESTAPI_HPP
