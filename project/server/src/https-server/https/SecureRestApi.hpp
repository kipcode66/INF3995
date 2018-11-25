#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include <common/logger/Logger.hpp>

#include "http-server/http/RestApi.hpp"
#include "descriptions/VolumeApi.hpp"
#include "descriptions/BlacklistApi.hpp"
#include "descriptions/StatsApi.hpp"
#include "descriptions/FileManagementApi.hpp"
#include "descriptions/AuthApi.hpp"

namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Pistache::Address addr, Logger& logger, FileCache& cache, Mp3EventClientSocket playerEventSocket);
    void init();

protected:
    Logger& m_logger;
    VolumeApi         m_volumeApi;
    BlacklistApi      m_blacklistApi;
    StatsApi          m_statsApi;
    FileManagementApi m_fileManagementApi;
    AuthApi           m_authApi;
};

} // namespace elevation
#endif // !HTTPS_RESTAPI_HPP

