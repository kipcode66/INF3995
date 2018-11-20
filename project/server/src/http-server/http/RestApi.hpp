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

namespace fs = std::experimental::filesystem;
namespace elevation {

class RestApi {
public:
    static constexpr const int MAX_SONG_PER_USER = 5;

protected:
    static User_t getUserFromRequestToken_(const Pistache::Rest::Request& request);
    static User_t extractUserDataFromRequest_(const Pistache::Rest::Request& request);

public:
    RestApi(Pistache::Address addr, Logger& logger, FileCache& cache, Mp3EventClientSocket playerEventSocket);
    ~RestApi();
    void init();
    void start();
    void setUpAutoPlayer();

protected:
    void createDescription_();
    void getIdentification_(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void getFileList_      (const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void postFile_         (const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
    void deleteFile_       (const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    std::string generateAllSongsAsViewedBy_(uint32_t token, bool adminSerialization = false);
    rapidjson::Value& generateSong_(const Song_t& song, uint32_t token, rapidjson::Document::AllocatorType& allocator, bool adminSerialization);

protected:
    std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
    Pistache::Rest::Description m_desc;
    Pistache::Rest::Router m_router;
    Logger& m_logger;
    FileCache& m_cache;
    Mp3EventClientSocket m_playerEventSocket;
};

} // namespace elevation

#endif // HTTP_RESTAPI_HPP
