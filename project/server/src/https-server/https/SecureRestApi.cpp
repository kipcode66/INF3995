#include "SecureRestApi.hpp"
#include "http-server/database/Database.hpp"
#include "rapidjson/document.h"
#include <pistache/serializer/rapidjson.h>
#include "http-server/http/RestApiUtils.hpp"

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr)
: RestApi(addr)
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
            || request_json["usager"] == '\n'
            || request_json["mot_de_passe"] == '\n') {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    char* login;
    strcpy(login, request_json["usager"].GetString());
    if (login != "admin") {
        response.send(Http::Code::Bad_Request, "Wrong login");
        return;
    }
    Database* db = Database::instance();
    std::vector<char*> saltAndHash = db->getSaltAndHashedPasswordByLogin(login);
    char* password;
    char* salt = saltAndHash[0];
    char* hash = saltAndHash[1];
    strcpy(password, request_json["mot_de_passe"].GetString());
    std::string passwordHash = elevation::restApiUtils::generateMd5Hash(password, salt);
    if (passwordHash == hash) {
        db->connectAdmin(login);
        response.send(Http::Code::Ok, "Connexion successful");
    } else {
        response.send(Http::Code::Forbidden, "Wrong password");
        return;
    }
}

void SecureRestApi::superviseurLogout_(const Rest::Request& request, Http::ResponseWriter response) {
    Database* db = Database::instance();
    if (!db->isAdminConnected()) {
        response.send(Http::Code::Unauthorized, "Admin not connected");
        return;
    } else {
        db->disconnectAdmin();
        response.send(Http::Code::Ok, "Logout successful");
    }
}

