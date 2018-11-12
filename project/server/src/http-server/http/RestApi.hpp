#ifndef HTTP_RESTAPI_HPP
#define HTTP_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <common/logger/Logger.hpp>
#include <database/Database.hpp>
#include <mp3/player/Mp3AutoPlayer.hpp>

#include "filesystem/FileCache.hpp"

#include <common/logger/Logger.hpp>

using namespace Pistache;
namespace fs = std::experimental::filesystem;
namespace elevation {

class RestApi {
public:
    static constexpr const int MAX_SONG_PER_USER = 5;

public:
    RestApi(Address addr, Logger& logger, FileCache& cache);
    ~RestApi();
    void init();
    void start();
    void setUpAutoPlayer();

protected:
    void createDescription_();
    void getIdentification_(const Rest::Request& request, Http::ResponseWriter response);
    void getFileList_(const Rest::Request& request, Http::ResponseWriter response);
    void postFile_(const Rest::Request& request, Http::ResponseWriter response);
    void deleteFile_(const Rest::Request& request, Http::ResponseWriter response);
    std::string generateSong_(const Song_t& song, uint32_t token);

    std::shared_ptr<Http::Endpoint> m_httpEndpoint;
    Rest::Description m_desc;
    Rest::Router m_router;
    Logger& m_logger;
    FileCache& m_cache;
};

} // namespace elevation

#endif // HTTP_RESTAPI_HPP
