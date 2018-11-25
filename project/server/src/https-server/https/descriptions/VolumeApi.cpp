#include "VolumeApi.hpp"

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include <common/mp3/event/VolumeChangeEvent.hpp>
#include <common/mp3/event/MuteEvent.hpp>
#include <common/mp3/event/UnmuteEvent.hpp>

#include <http-server/http/exception/BadRequestException.hpp>
#include <http-server/http/exception/InvalidTokenException.hpp>

#include "Admin.hpp"
#include "https/exception/AuthenticationFailureException.hpp"

using namespace Pistache;

namespace elevation {

VolumeApi::VolumeApi(Pistache::Rest::Description& desc, Logger& logger, std::shared_ptr<Mp3EventClientSocket> playerEventSocket, HttpsServerEventFacade& eventFacade)
    : m_logger(logger)
    , m_playerEventSocket(playerEventSocket)
    , m_eventFacade(eventFacade)
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
    try {
        if (isAdminAuthenticated_(request, response)) {
            VolumeData_t volumeData = m_eventFacade.getVolumeData();
            rapidjson::Document songsDoc;
            songsDoc.SetObject();
            songsDoc.AddMember(rapidjson::Value().SetString("volume"  ), volumeData.volume , songsDoc.GetAllocator());
            songsDoc.AddMember(rapidjson::Value().SetString("sourdine"), volumeData.isMuted, songsDoc.GetAllocator());

            rapidjson::StringBuffer buf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
            songsDoc.Accept(writer);

            std::string serializedVolumeData = buf.GetString();
            response.send(Http::Code::Ok, serializedVolumeData);
        }
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

void VolumeApi::POST_volumeAssigner_ (const Rest::Request& request,
                                               Http::ResponseWriter response) {
    volumePercent_t newVolume;
    try {
        unsigned int tmpVolume = 0;
        if (!(std::istringstream{request.param(":volume").as<std::string>()} >> tmpVolume) || tmpVolume > 100) {
            throw std::runtime_error("");
        }
	newVolume = tmpVolume;
    }
    catch (const std::runtime_error& e) {
        response.send(Pistache::Http::Code::Bad_Request, "Volume must be an integer and must be in range [0, 100]");
        return;
    }

    try {
        if (isAdminAuthenticated_(request, response)) {
            VolumeChangeEvent event{newVolume};
            m_playerEventSocket->writeEvent(event);
            response.send(Http::Code::Ok, "Volume change commiting...");
        }
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

void VolumeApi::POST_sourdineActiver_(const Rest::Request& request,
                                                    Http::ResponseWriter response) {
    try {
        if (isAdminAuthenticated_(request, response)) {
            MuteEvent event;
            m_playerEventSocket->writeEvent(event);
            response.send(Http::Code::Ok, "Mute commiting...");
        }
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

void VolumeApi::POST_sourdineDesactiver_(const Rest::Request& request,
                                                       Http::ResponseWriter response) {
    try {
        if (isAdminAuthenticated_(request, response)) {
            UnmuteEvent event;
            m_playerEventSocket->writeEvent(event);
            response.send(Http::Code::Ok, "Unmute commiting...");
        }
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error);
        return;
    }
}

bool VolumeApi::isAdminAuthenticated_(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter& response) {
    Pistache::Http::Code errorCode;
    std::string errorMessage;
    try {
        volatile Admin admin = Admin::getAdminDataFromRequestToken(request);
        return true;
    }
    catch (const BadRequestException& e) {
        errorCode = Pistache::Http::Code::Bad_Request;
        errorMessage = e.what();
    }
    catch (const InvalidTokenException& e) {
        errorCode = Pistache::Http::Code::Forbidden;
        errorMessage = e.what();
    }
    catch (const AuthenticationFailureException& e) {
        errorCode = Pistache::Http::Code::Unauthorized;
        errorMessage = e.what();
    }
    std::ostringstream logMessageString;
    logMessageString << request.method() << ' ' << request.resource() << " failed: " << errorMessage;
    m_logger.err(logMessageString.str());
    response.send(errorCode, errorMessage);
    return false;
}

} // namespace elevation
