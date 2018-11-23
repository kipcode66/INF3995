/* #include <pistache/net.h> */
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "StatsApi.hpp"

using namespace Pistache;

namespace elevation {

StatsApi::StatsApi(Rest::Description& desc) {
    auto superviseurPath = desc.path("/superviseur");
    superviseurPath
            .route(desc.get("/statistiques"))
            .bind(&StatsApi::getSuperviseurStatistiques_, this)
            .hide();
}

void StatsApi::getSuperviseurStatistiques_(const Rest::Request& request,
                                           Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "statistics");
}

} // namespace elevation
