#ifndef HTTPS_DESCRIPTIONS_STATSAPI
#define HTTPS_DESCRIPTIONS_STATSAPI

#include <pistache/description.h>
#include <common/database/Database.hpp>

class StatsApi {
public:
    StatsApi(Pistache::Rest::Description& desc);

private:
    void getSuperviseurStatistiques_( const Pistache::Rest::Request&,
                                      Pistache::Http::ResponseWriter);

    std::string generateStatistics_(const Statistics&);
};
#endif // !HTTPS_DESCRIPTIONS_STATSAPI

