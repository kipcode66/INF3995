#include "BlacklistApi.hpp"

using namespace Pistache;

BlacklistApi::BlacklistApi(Pistache::Rest::Description& desc) {
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
        .route(desc.post("/bloquer"))
        .bind(&BlacklistApi::postSuperviseurBloquer_, this)
        .hide();

    superviseurPath
        .route(desc.post("/debloquer"))
        .bind(&BlacklistApi::postSuperviseurDebloquer_, this)
        .hide();

    superviseurPath
        .route(desc.get("/listenoire"))
        .bind(&BlacklistApi::getSuperviseurListenoire_, this)
        .hide();
}

void BlacklistApi::postSuperviseurBloquer_(const Rest::Request& request,
                                            Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user blocked");
}

void BlacklistApi::postSuperviseurDebloquer_(const Rest::Request& request,
                                              Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "user unblocked");
}

void BlacklistApi::getSuperviseurListenoire_(const Rest::Request& request,
                                             Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "liste noire");
}

