#include "SecureRestApi.hpp"

using namespace elevation;

void SecureRestApi::createStatisticsDescription_(Pistache::Rest::Description& desc) {
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/statistiques"))
            .bind(&SecureRestApi::getSuperviseurStatistiques_, this)
            .hide();
}

void SecureRestApi::getSuperviseurStatistiques_(const Rest::Request& request,
                                 Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "statistics");
}

