#include "SecureRestApi.hpp"

using namespace elevation;

void SecureRestApi::getSuperviseurStatistiques_(const Rest::Request& request,
                                 Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "statistics");
}

