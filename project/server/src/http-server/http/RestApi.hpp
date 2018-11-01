#ifndef HTTP_RESTAPI_HPP
#define HTTP_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <common/logger/Logger.hpp>

#include "filesystem/FileCache.hpp"

using namespace Pistache;
namespace elevation {

class RestApi {
public:
    RestApi(Address);
    ~RestApi();
    void init();
    void start();

protected:
    void createDescription_();
    void getIdentification_(const Rest::Request& request, Http::ResponseWriter response);
    void getFileList_(const Rest::Request& request, Http::ResponseWriter response);
    void postFile_(const Rest::Request& request, Http::ResponseWriter response);
    void deleteFile_(const Rest::Request& request, Http::ResponseWriter response);

    std::shared_ptr<Http::Endpoint> m_httpEndpoint;
    Rest::Description m_desc;
    Rest::Router m_router;
    FileCache m_cache;
    Logger& m_logger;
};

} // namespace elevation

#endif // HTTP_RESTAPI_HPP
