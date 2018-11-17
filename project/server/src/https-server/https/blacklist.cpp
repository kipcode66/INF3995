#include "SecureRestApi.hpp"

using namespace elevation;

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

