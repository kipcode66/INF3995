#include "SecureRestApi.hpp"
#include "http-server/database/Database.hpp"
#include "rapidjson/document.h"

#include <common/logger/Logger.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr)
: RestApi(addr)
, m_logger(Logger::getLogger("https-server"))
{ }

void SecureRestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
    createSecureDescription_();
}

void SecureRestApi::createSecureDescription_() {
    auto superviseurPath = m_desc.path("/superviseur");

    superviseurPath
            .route(m_desc.get("/file"))
            .bind(&SecureRestApi::getSuperviseurFile_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/changement_mdp"))
            .bind(&SecureRestApi::postChangePassword_, this)
            .hide();
}

void SecureRestApi::getSuperviseurFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::cout << "getSuperviseurFile called " << std::endl;
}

void SecureRestApi::postChangePassword_(const Rest::Request& request, Http::ResponseWriter response) {
    try {
        auto body = request.body();
        rapidjson::Document request_json;
        request_json.Parse(body.c_str());
        if (!request_json.HasMember("ancien") || !request_json.HasMember("nouveau")) {
            response.send(Http::Code::Bad_Request, "Malformed request");
            return;
        }
        std::string ancien(request_json["ancien"].GetString());
        std::string nouveau(request_json["nouveau"].GetString());
        std::cout << "changing " << ancien << " to " << nouveau  << std::endl;
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
    }
    response.send(Http::Code::Ok, "chgm_mdp");
}
