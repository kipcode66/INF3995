#include "VolumeApi.hpp"

using namespace Pistache;

VolumeApi::VolumeApi(Pistache::Rest::Description& desc) {
    auto volumePath = desc.path("superviseur/volume");
    volumePath
            .route(desc.get(""))
            .bind(&VolumeApi::GET_volume_, this)
            .hide();

    volumePath
            .route(desc.post("/augmenter/:pc"))
            .bind(&VolumeApi::POST_volumeAug_, this)
            .hide();

    volumePath
            .route(desc.post("/diminuer/:pc"))
            .bind(&VolumeApi::POST_volumeDim_, this)
            .hide();

    auto sourdinePath = volumePath.path("/sourdine");
    sourdinePath
            .route(desc.post("/activer"))
            .bind(&VolumeApi::POST_sourdineActiver_, this)
            .hide();

    sourdinePath
            .route(desc.post("/desactiver"))
            .bind(&VolumeApi::POST_sourdineDesactiver_, this)
            .hide();
}

void VolumeApi::GET_volume_(const Rest::Request& request,
                                          Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "GET_volume_ called");
}

void VolumeApi::POST_volumeAug_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Up");
}

void VolumeApi::POST_volumeDim_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Down");
}

void VolumeApi::POST_sourdineActiver_(const Rest::Request& request,
                                                    Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume muted");
}

void VolumeApi::POST_sourdineDesactiver_(const Rest::Request& request,
                                                       Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume unmuted");
}

