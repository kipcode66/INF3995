#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>

#include "SecureRestApi.hpp"
#include "rapidjson/document.h"
#include "http-server/http/RestApiUtils.hpp"
#include "database/Database.hpp"


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
            .route(m_desc.post("/login"))
            .bind(&SecureRestApi::superviseurLogin_, this)
            .hide();

    superviseurPath
            .route(m_desc.post("/logout"))
            .bind(&SecureRestApi::superviseurLogout_, this)
            .hide();
}

void SecureRestApi::getSuperviseurFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurFile called");
    std::cout << "getSuperviseurFile called " << std::endl;
}

void SecureRestApi::superviseurLogin_(const Rest::Request& request, Http::ResponseWriter response) {
    auto body = request.body();
    rapidjson::Document request_json;
    request_json.Parse(body.c_str());
    if (!request_json.HasMember("usager")
            || !request_json.HasMember("mot_de_passe")
            || !request_json.HasMember("X-Auth-Token")
            || request_json["usager"] == '\n'
            || request_json["mot_de_passe"] == '\n'
            || request_json["X-Auth-Token"] == '\n') {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    std::string login(request_json["usager"].GetString());
    if (login != "admin") {
        response.send(Http::Code::Bad_Request, "Wrong login");
        return;
    }
    uint32_t admin_id = request_json["X-Auth-Token"].GetUint();
    Database* db = Database::instance();
    if (db->isAdminConnected(admin_id)) {
        response.send(Http::Code::Bad_Request, "Admin already connected with this token");
        return;
    }
    std::vector<char*> saltAndHash = db->getSaltAndHashedPasswordByLogin(login.c_str());
    char* salt = saltAndHash[0];
    char* hash = saltAndHash[1];
    std::string password(request_json["mot_de_passe"].GetString());
    std::string passwordHash = elevation::restApiUtils::generateMd5Hash(password, salt);
    if (passwordHash == hash) {
        db->connectAdmin(login.c_str(), admin_id);
        response.send(Http::Code::Ok, "Connexion successful");
        return;
    } else {
        response.send(Http::Code::Forbidden, "Wrong password");
        return;
    }
}

void SecureRestApi::superviseurLogout_(const Rest::Request& request, Http::ResponseWriter response) {
    auto body = request.body();
    rapidjson::Document request_json;
    request_json.Parse(body.c_str());
    if (!request_json.HasMember("X-Auth-Token")
            || request_json["X-Auth-Token"] == '\n') {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    uint32_t admin_id = request_json["X-Auth-Token"].GetUint();
    Database* db = Database::instance();
    if (!db->isAdminConnected(admin_id)) {
        response.send(Http::Code::Unauthorized, "Admin not connected");
        return;
    } else {
        db->disconnectAdmin(admin_id);
        response.send(Http::Code::Ok, "Logout successful");
        return;
    }
}

