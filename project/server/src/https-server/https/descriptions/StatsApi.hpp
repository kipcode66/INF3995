#ifndef HTTPS_DESCRIPTIONS_STATSAPI
#define HTTPS_DESCRIPTIONS_STATSAPI

#include <pistache/description.h>

namespace elevation {

class StatsApi {
public:
    StatsApi(Pistache::Rest::Description& desc);

private:
    void getSuperviseurStatistiques_( const Pistache::Rest::Request&,
                                      Pistache::Http::ResponseWriter);
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_STATSAPI

