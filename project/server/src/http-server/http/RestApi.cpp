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

void buildUserFromQuery_(User_t* __restrict__ requestUser,
                         Pistache::Http::Uri::Query* __restrict__ query) {
    strcpy(requestUser->mac, query->get("mac").get().c_str());
    strcpy(requestUser->ip, query->get("ip").get().c_str());
    strcpy(requestUser->name, query->get("name").get().c_str());
}

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {
    puts("getIdentification function called");

    auto body = request.body();
    rapidjson::Document document;
    document.Parse(body.c_str());

    User_t requestUser = { 0 };
    if (!document.HasMember("mac")
            || !document.HasMember("ip")
            || !document.HasMember("name")
            || document["mac"] == '\n'
            || document["ip"] == '\n') {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    strcpy(requestUser.mac, document["mac"].GetString());
    strcpy(requestUser.ip, document["ip"].GetString());
    strcpy(requestUser.name, document["name"].GetString());

    std::string mac(requestUser.mac);

    User_t oldUser = { 0 };

    Database* db = Database::instance();
    db->getUserByMac(mac.c_str(), &oldUser);
    if (*oldUser.mac == 0) {
        db->createUser(&requestUser);

        /* rapidjson::Document response_json; */
        /* response_json.SetObject(); */
        /* auto& allocator = response_json.GetAllocator(); */
        /* response_json.AddMember("id", 345, allocator); */
        /* response_json.AddMember("message", "connection succesful", allocator); */
        /* std::string s(response_json.GetString()); */
        /* std::cout << s << std::endl; */
        std::string body("{\"id\": \"asdfasdfasdf\", \"message\": \"connection succesfull\" ");
        response.send(Http::Code::Ok, body);
    } else {
        requestUser.id = oldUser.id;
        if (db->createUser(&requestUser)) {
            response.send(Http::Code::Internal_Server_Error, "couldn't create user in db");
        } else {
        std::string body("{\"id\": \"asdfasdfasdf\", \"message\": \"connection succesfull\"}");
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
