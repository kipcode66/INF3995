#ifndef HTTPS_RESTAPI_HPP
#define HTTPS_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

#include "http-server/http/RestApi.hpp"
#include "descriptions/VolumeApi.hpp"
#include "descriptions/BlacklistApi.hpp"
#include "descriptions/StatsApi.hpp"
#include "descriptions/FileManagementApi.hpp"
#include "descriptions/AuthApi.hpp"

#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace elevation {

class SecureRestApi : public RestApi {
public:
    SecureRestApi(Address addr, Logger& logger, FileCache&);
    void init();

private:

    VolumeApi         m_volumeApi;
    BlacklistApi      m_blacklistApi;
    StatsApi          m_statsApi;
    AuthApi           m_authApi;
    FileManagementApi m_fileManagementApi;

private:
    Logger& m_logger;
};

} // namespace elevation

#endif // !HTTPS_RESTAPI_HPP
