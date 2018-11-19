#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "http-server/http/RestApi.hpp"
#include "descriptions/VolumeApi.hpp"
#include "descriptions/BlacklistApi.hpp"
#include "descriptions/StatsApi.hpp"
#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Address addr, Logger& logger, FileCache&);
    void init();

private:

    void createStatisticsDescription_      (Rest::Description&);
    void createAuthDescription_            (Rest::Description&);
    void createFileManagementDescription_  (Rest::Description&);

    VolumeApi      m_volumeApi;
    BlacklistApi   m_blacklistApi;
    StatsApi       m_statsApi;
    /* AuthApi           m_volumeApi; */
    /* FileManagementApi m_volumeApi; */

    void getSuperviseurFile_               (const Rest::Request&, Http::ResponseWriter);
    void deleteSuperviseurChanson_         (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurInversion_         (const Rest::Request&, Http::ResponseWriter);

    void getSuperviseurStatistiques_       (const Rest::Request&, Http::ResponseWriter);


    void postChangePassword_               (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogin_                 (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogout_                (const Rest::Request&, Http::ResponseWriter);

private:
    Logger& m_logger;
};

} // namespace elevation

#endif // !HTTPS_RESTAPI_HPP
