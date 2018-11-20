#ifndef HTTP_RESTAPI_HPP
#define HTTP_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <rapidjson/document.h>

#include <common/logger/Logger.hpp>
#include <database/Database.hpp>

#include <common/filesystem/FileCache.hpp>
#include <common/logger/Logger.hpp>
#include <common/mp3/communication/Mp3EventClientSocket.hpp>

using namespace Pistache;
namespace fs = std::experimental::filesystem;
namespace elevation {

class RestApi {
public:
    static constexpr const int MAX_SONG_PER_USER = 5;

protected:
    static User_t getUserFromRequestToken_(const Rest::Request& request);
    static User_t extractUserDataFromRequest_(const Rest::Request& request);

public:
    RestApi(Address addr, Logger& logger, FileCache& cache, Mp3EventClientSocket playerEventSocket);
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

    std::string generateAllSongsAsViewedBy_(uint32_t token, bool adminSerialization = false);
    rapidjson::Value& generateSong_(const Song_t& song, uint32_t token, rapidjson::Document::AllocatorType& allocator, bool adminSerialization);

    std::shared_ptr<Http::Endpoint> m_httpEndpoint;
    Rest::Description m_desc;
    Rest::Router m_router;
    Logger& m_logger;
    FileCache& m_cache;
    Mp3EventClientSocket m_playerEventSocket;
};

} // namespace elevation

#endif // HTTP_RESTAPI_HPP
