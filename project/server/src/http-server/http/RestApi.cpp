//
// Created by adam on 04/10/18.
//

#include "RestApi.hpp"

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

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {
    auto token = request.headers().getRaw("X-Auth-Token");
    uint32_t t = std::stoi(token.value());
    if (t == 0) {
        t = rand();
    }
    response.send(Http::Code::Ok, "{\"identificateur\": " + std::to_string(t) + ", \"message\":\"Bienvenue au café-bistro Élévation!\"}");
    printf("getIdentification function called, token is %d\n", t);
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::string param = request.param(":id").as<std::string>();
    response.send(Http::Code::Ok, "{\n\"chansons\":[\n"
        "{\n\"titre\":\"Never Gonna Give You Up\",\n\"artiste\":\"Foo\",\n\"duree\":\"4:20\",\n\"proposeePar\":\"Chuck Norris\",\n\"proprietaire\":false,\n\"no\":42},\n"
        "{\n\"titre\":\"Hey Jude\",\n\"artiste\":\"Beatles\",\n\"duree\":\"7:05\",\n\"proposeePar\":\"Claude\",\n\"proprietaire\":true,\n\"no\":25}\n"
    "]\n}\n");
    printf("getFileList function called, param is %s\n", param.c_str());
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postFile");
    puts("postFile function called");
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    puts("deleteFile function called");
}
