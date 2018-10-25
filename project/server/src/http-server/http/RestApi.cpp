#include <pistache/serializer/rapidjson.h>

#include "RestApi.hpp"
#include "database/Database.hpp"
#include "rapidjson/document.h"

using namespace elevation;

RestApi::RestApi(Address addr)
: m_httpEndpoint(std::make_shared<Http::Endpoint>(addr))
, m_desc("Rest API", "1.0")
{ }

RestApi::~RestApi() {
    m_httpEndpoint->shutdown();
}

void RestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
}

void RestApi::start() {
    m_router.initFromDescription(m_desc);

    Rest::Swagger swagger(m_desc);
    swagger
            .install(m_router);

    m_httpEndpoint->setHandler(m_router.handler());
    m_httpEndpoint->serve();
}

void RestApi::createDescription_() {
    m_desc
            .info()
            .license("Apache", "http://www.apache.org/licenses/LICENSE-2.0");

    auto backendErrorResponse =
            m_desc.response(Http::Code::Internal_Server_Error, "An error occurred with the backend");

    m_desc
            .schemes(Rest::Scheme::Http)
            .produces(MIME(Application, Json))
            .consumes(MIME(Application, Json));

    auto usagerPath = m_desc.path("/usager");

    usagerPath
            .route(m_desc.get("/identification"))
            .bind(&RestApi::getIdentification_, this)
            .hide();

    usagerPath
            .route(m_desc.get("/file/:id"))
            .bind(&RestApi::getFileList_, this)
            .hide();

    usagerPath
            .route(m_desc.post("/chanson/:id"))
            .bind(&RestApi::postFile_, this)
            .hide();

    usagerPath
            .route(m_desc.del("/chanson/:id/:no"))
            .bind(&RestApi::deleteFile_, this)
            .hide();
}


std::string generateBody(std::string id, std::string message) {
    char jsonString[] = "{\"id\": \"%s\", \"message\": \"connection successful\"}";
    size_t stringSize = sizeof(jsonString) + sizeof(id);
    char* buffer = (char *)calloc(1, stringSize);
    snprintf(buffer, stringSize, jsonString, id.c_str());
    std::string body(buffer);
    free(buffer);
    return body;
}

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {

    auto body = request.body();
    rapidjson::Document request_json;
    request_json.Parse(body.c_str());

    if (!request_json.HasMember("mac")
            || !request_json.HasMember("ip")
            || !request_json.HasMember("name")
            || request_json["mac"] == '\n'
            || request_json["ip"] == '\n') {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    User_t requestUser = { 0 };
    strcpy(requestUser.mac, request_json["mac"].GetString());
    strcpy(requestUser.ip, request_json["ip"].GetString());
    strcpy(requestUser.name, request_json["name"].GetString());

    User_t existingUser = { 0 };
    Database* db = Database::instance();
    db->getUserByMac(requestUser.mac, &existingUser);
    if (*existingUser.mac == 0) {
        db->createUser(&requestUser);

        // MOCK id TODO generate and insert in db
        char id[]= "asKJd8hq*l#Dcdac_&Hgfasdf896gq34";
        std::string body = generateBody(id, "connection successful");
        response.send(Http::Code::Ok, body);
    } else {
        /* requestUser.id = existingUser.id; */
        if (db->createUser(&requestUser)) {
            response.send(Http::Code::Internal_Server_Error, "couldn't create user in db");
        } else {
            char id[]= "asKJd8hq*l#Dcdac_&Hgfasdf896gq34";
            std::string body = generateBody(id, "connection successful");
            response.send(Http::Code::Ok, body);
        }
    }
    response.send(Http::Code::Ok, "getIdentification called");
    return;
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::string param = request.param(":id").as<std::string>();
    response.send(Http::Code::Ok, "getFileList, param is : " + param);
    std::cout << "getFileList function called, param is " << param << std::endl;
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postFile");
    std::cout << "postFile function called" << std::endl;
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    std::cout << "deleteFile function called" << std::endl;
}
