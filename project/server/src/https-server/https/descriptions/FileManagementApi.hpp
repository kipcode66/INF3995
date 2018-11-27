#ifndef HTTPS_DESCRIPTIONS_FILEAPI
#define HTTPS_DESCRIPTIONS_FILEAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>
#include <common/filesystem/FileCache.hpp>
#include <common/mp3/communication/Mp3EventClientSocket.hpp>

#include "mp3/event/HttpsServerEventFacade.hpp"

namespace elevation {

class FileManagementApi {
public:
    FileManagementApi(Pistache::Rest::Description&,
                      elevation::Logger& logger,
                      elevation::FileCache&,
                      std::shared_ptr<Mp3EventClientSocket> playerEventSocket,
                      HttpsServerEventFacade& eventFacade);

private:
    void getSuperviseurFile_       (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void deleteSuperviseurChanson_ (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void postSuperviseurInversion_ (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);

    elevation::Logger& m_logger;
    elevation::FileCache& m_cache;
    std::shared_ptr<Mp3EventClientSocket> m_playerEventSocket;
    HttpsServerEventFacade& m_eventFacade;
};

} // namespace elevation

#endif // !HTTPS_DESCRIPTIONS_FILEAPI

