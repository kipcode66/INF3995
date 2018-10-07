//
// Created by adam on 04/10/18.
//

#include "RestApi.hpp"

RestApi::RestApi(Address addr)
: httpEndpoint(std::make_shared<Http::Endpoint>(addr))
, desc("Rest API", "1.0")
{ }

void RestApi::init() {
    auto opts = Http::Endpoint::options();
    httpEndpoint->init(opts);
    createDescription();
}

void RestApi::start() {
    router.initFromDescription(desc);

    Rest::Swagger swagger(desc);
    swagger
            .uiPath("/doc")
            .uiDirectory("/home/octal/code/web/swagger-ui-2.1.4/dist")
            .apiPath("/banker-api.json")
            .install(router);

    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void RestApi::shutdown() {
    httpEndpoint->shutdown();
}

void RestApi::createDescription() {
    desc
            .info()
            .license("Apache", "http://www.apache.org/licenses/LICENSE-2.0");

    auto backendErrorResponse =
            desc.response(Http::Code::Internal_Server_Error, "An error occurred with the backend");

    desc
            .schemes(Rest::Scheme::Http)
            .produces(MIME(Application, Json))
            .consumes(MIME(Application, Json));

    desc
            .route(desc.get("/usager/identification"), "request a user authentication")
            .bind(&RestApi::getIdentification, this)
            .response(Http::Code::Ok, "Response to the /usager/identification call")
            .hide();
}

void RestApi::getIdentification(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "identification");
    puts("getIdentification");
}

void RestApi::getFileList(const Rest::Request& request, Http::ResponseWriter response) {

}

void RestApi::postFile(const Rest::Request& request, Http::ResponseWriter response) {

}

void RestApi::deleteFile(const Rest::Request& request, Http::ResponseWriter response) {

}
