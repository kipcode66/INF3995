#include "SecureRestApi.hpp"

using namespace elevation;


void SecureRestApi::createVolumeDescription_(Pistache::Rest::Description& desc) {
    auto volumePath = desc.path("/volume");
    volumePath
            .route(desc.get(""))
            .bind(&SecureRestApi::getSuperviseurVolume_, this)
            .hide();

    volumePath
            .route(desc.post("/augmenter/:pc"))
            .bind(&SecureRestApi::postSuperviseurVolumeAug_, this)
            .hide();

    volumePath
            .route(desc.post("/diminuer/:pc"))
            .bind(&SecureRestApi::postSuperviseurVolumeDim_, this)
            .hide();

    auto sourdinePath = volumePath.path("/sourdine");
    sourdinePath
            .route(desc.post("/activer"))
            .bind(&SecureRestApi::postSuperviseurSourdineActiver_, this)
            .hide();

    sourdinePath
            .route(desc.post("/desactiver"))
            .bind(&SecureRestApi::postSuperviseurSourdineDesactiver_, this)
            .hide();
}

void SecureRestApi::getSuperviseurVolume_(const Rest::Request& request,
                                          Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurVolume_ called");
}

void SecureRestApi::postSuperviseurVolumeAug_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Up");
}

void SecureRestApi::postSuperviseurVolumeDim_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Down");
}

void SecureRestApi::postSuperviseurSourdineActiver_(const Rest::Request& request,
                                                    Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume muted");
}

void SecureRestApi::postSuperviseurSourdineDesactiver_(const Rest::Request& request,
                                                       Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume unmuted");
}
