#include "SecureRestApi.hpp"

using namespace elevation;


void SecureRestApi::createBlacklistDescription_(Pistache::Rest::Description& desc) {
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
        .route(desc.post("/bloquer"))
        .bind(&SecureRestApi::postSuperviseurBloquer_, this)
        .hide();

    superviseurPath
        .route(desc.post("/debloquer"))
        .bind(&SecureRestApi::postSuperviseurDebloquer_, this)
        .hide();

    superviseurPath
        .route(desc.get("/listenoire"))
        .bind(&SecureRestApi::getSuperviseurListenoire_, this)
        .hide();
}

void SecureRestApi::postSuperviseurBloquer_(const Rest::Request& request,
                                            Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user blocked");
}

void SecureRestApi::postSuperviseurDebloquer_(const Rest::Request& request,
                                              Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user unblocked");
}

void SecureRestApi::getSuperviseurListenoire_(const Rest::Request& request,
                                             Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "liste noire");
}

