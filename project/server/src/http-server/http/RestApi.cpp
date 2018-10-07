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

    auto usagerPath = desc.path("/usager");

    usagerPath
            .route(desc.get("/identification"))
            .bind(&RestApi::getIdentification, this)
            .response(Http::Code::Ok, "")
            .hide();

    usagerPath
            .route(desc.get("/file/:id"))
            .bind(&RestApi::getFileList, this)
            .response(Http::Code::Ok, "")
            .hide();

    usagerPath
            .route(desc.post("/chanson/:id"))
            .bind(&RestApi::postFile, this)
            .response(Http::Code::Ok, "")
            .hide();

    usagerPath
            .route(desc.del("/chanson/:id"))
            .bind(&RestApi::deleteFile, this)
            .response(Http::Code::Ok, "")
            .hide();
}

void RestApi::getIdentification(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getIdentification");
    puts("getIdentification function called");
}

void RestApi::getFileList(const Rest::Request& request, Http::ResponseWriter response) {
    std::string param = request.param(":id").as<std::string>();
    response.send(Http::Code::Ok, "getFileList, param is : " + param);
    printf("getFileList function called, param is %s\n", param.c_str());
}

void RestApi::postFile(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postFile");
    puts("postFile function called");
}

void RestApi::deleteFile(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    puts("deleteFile function called");
}
