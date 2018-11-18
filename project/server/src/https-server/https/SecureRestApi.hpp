#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "http-server/http/RestApi.hpp"

#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Address addr, Logger& logger, FileCache&);
    void init();

private:
    void createSecureDescription_    ( );
    void createVolumeDescription_    ( Pistache::Rest::Description&);
    void createBlacklistDescription_ ( Pistache::Rest::Description&);

    void getSuperviseurFile_               (const Rest::Request&, Http::ResponseWriter);
    void deleteSuperviseurChanson_         (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurInversion_         (const Rest::Request&, Http::ResponseWriter);

    void getSuperviseurVolume_             (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurVolumeAug_         (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurVolumeDim_         (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurSourdineActiver_   (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurSourdineDesactiver_(const Rest::Request&, Http::ResponseWriter);

    void getSuperviseurStatistiques_       (const Rest::Request&, Http::ResponseWriter);

    void postSuperviseurBloquer_           (const Rest::Request&, Http::ResponseWriter);
    void postSuperviseurDebloquer_         (const Rest::Request&, Http::ResponseWriter);
    void getSuperviseurListenoire_         (const Rest::Request&, Http::ResponseWriter);

    void postChangePassword_               (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogin_                 (const Rest::Request&, Http::ResponseWriter);
    void superviseurLogout_                (const Rest::Request&, Http::ResponseWriter);

private:
    Logger& m_logger;

    class Admin {
    public:
        Admin(const Rest::Request&);

        std::string usager;
        std::string mot_de_passe;
        uint32_t    id;
    };
};

} // namespace elevation

#endif // !HTTPS_RESTAPI_HPP
