#ifndef HTTPS_DESCRIPTIONS_STATSAPI
#define HTTPS_DESCRIPTIONS_STATSAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>

namespace elevation {

class StatsApi {
public:
    StatsApi(Pistache::Rest::Description& desc, Logger&);

private:
    void getSuperviseurStatistiques_( const Pistache::Rest::Request&,
                                      Pistache::Http::ResponseWriter);

private:
    Logger& m_logger;
};
}
#endif // !HTTPS_DESCRIPTIONS_STATSAPI

