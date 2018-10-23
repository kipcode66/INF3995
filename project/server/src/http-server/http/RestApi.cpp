#include <pistache/serializer/rapidjson.h>

#include "RestApi.hpp"
#include "database/Database.hpp"

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

void buildUserFromQuery_(User_t* __restrict__ newUser,
                         Pistache::Http::Uri::Query* __restrict__ query) {
    strcpy(newUser->mac, query->get("mac").get().c_str());
    strcpy(newUser->ip, query->get("ip").get().c_str());
    strcpy(newUser->name, query->get("name").get().c_str());
}

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {

    auto body = request.body();
    std::cout << body << std::endl;
    puts("getIdentification function called");

    auto query = request.query();
    if (!query.has("mac")) {
        response.send(Http::Code::Bad_Request, "Malformed request");
    } else {
        std::string mac(query.get("mac").get());

        User_t newUser = { 0 };
        User_t oldUser = { 0 };

        Database* db = Database::instance();
        db->getUserByMac(mac.c_str(), &oldUser);
        if (*oldUser.mac == 0) {
            buildUserFromQuery_(&newUser, &query);
            db->createUser(&newUser);
            response.send(Http::Code::Ok, "New user created, id=" + std::to_string(newUser.id) + "\n");
        } else {
            buildUserFromQuery_(&newUser, &query);
            newUser.id = oldUser.id;
            if (db->createUser(&newUser)) {
                response.send(Http::Code::Internal_Server_Error, "couldn't create user in db");
            } else {
                response.send(Http::Code::Ok, "this user exist, id=" + std::to_string(newUser.id) + "\n");
            }
        }
        response.send(Http::Code::Ok, "getIdentification called");
    }

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
