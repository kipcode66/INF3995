#include "FileManagementApi.hpp"

using namespace Pistache;

FileManagementApi::FileManagementApi(Rest::Description& desc, Logger& logger)
    : m_logger(logger)
{
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/file"))
            .bind(&FileManagementApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(desc.del("/chanson/:no"))
            .bind(&FileManagementApi::deleteSuperviseurChanson_, this)
            .hide();

    superviseurPath
            .route(desc.post("/inversion"))
            .bind(&FileManagementApi::postSuperviseurInversion_, this)
            .hide();
}

void FileManagementApi::getSuperviseurFile_(const Rest::Request& request,
                                            Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::ostringstream logMsg;
    logMsg << "getSuperviseurFile called ";
    m_logger.log(logMsg.str());
}

void FileManagementApi::deleteSuperviseurChanson_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteSuperviseurChanson called");
}

void FileManagementApi::postSuperviseurInversion_(const Rest::Request& request,
                                                  Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postSuperviseurInversion called");
}

