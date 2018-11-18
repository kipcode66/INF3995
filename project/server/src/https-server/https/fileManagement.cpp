#include <rapidjson/document.h>
#include <common/logger/Logger.hpp>

#include "SecureRestApi.hpp"

using namespace elevation;


void SecureRestApi::createFileManagementDescription_(Pistache::Rest::Description& desc) {
    auto superviseurPath = m_desc.path("/superviseur");
    superviseurPath
            .route(m_desc.get("/file"))
            .bind(&SecureRestApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(m_desc.del("/chanson/:no"))
            .bind(&SecureRestApi::deleteSuperviseurChanson_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/inversion"))
            .bind(&SecureRestApi::postSuperviseurInversion_, this)
            .hide();
}

void SecureRestApi::getSuperviseurFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::ostringstream logMsg;
    logMsg << "getSuperviseurFile called ";
    m_logger.log(logMsg.str());
}

void SecureRestApi::deleteSuperviseurChanson_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteSuperviseurChanson called");
}

void SecureRestApi::postSuperviseurInversion_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postSuperviseurInversion called");
}

