#include "VolumeApi.hpp"

#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <common/mp3/event/MuteEvent.hpp>
#include <common/mp3/event/UnmuteEvent.hpp>

using namespace Pistache;

namespace elevation {

VolumeApi::VolumeApi(Pistache::Rest::Description& desc, Logger& logger, Mp3EventClientSocket playerEventSocket)
    : m_logger(logger)
    , m_playerEventSocket(std::move(playerEventSocket))
{
    auto volumePath = desc.path("superviseur/volume");
    volumePath
            .route(desc.get(""))
            .bind(&VolumeApi::GET_volume_, this)
            .hide();

    volumePath
            .route(desc.post("/assigner/:volume"))
            .bind(&VolumeApi::POST_volumeAssigner_, this)
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

void VolumeApi::POST_volumeAssigner_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    volumePercent_t newVolume;
    try {
        newVolume = request.param("volume").as<volumePercent_t>();
        if (newVolume > 100) {
            throw std::runtime_error("");
        }
    }
    catch (const std::runtime_error& e) {
        response.send(Pistache::Http::Code::Bad_Request, "Volume must be an integer and must be in range [0, 100]");
        return;
    }

    try {
        VolumeChangeEvent event{newVolume};
        m_playerEventSocket.write(event);
        response.send(Http::Code::Ok, "Volume change commiting...");
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

void VolumeApi::POST_sourdineActiver_(const Rest::Request& request,
                                                    Http::ResponseWriter response) {
    try {
        MuteEvent event;
        m_playerEventSocket.write(event);
        response.send(Http::Code::Ok, "Mute commiting...");
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

void VolumeApi::POST_sourdineDesactiver_(const Rest::Request& request,
                                                       Http::ResponseWriter response) {
    try {
        UnmuteEvent event;
        m_playerEventSocket.write(event);
        response.send(Http::Code::Ok, "Unmute commiting...");
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

} // namespace elevation
