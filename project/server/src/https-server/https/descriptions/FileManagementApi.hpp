#ifndef HTTPS_DESCRIPTIONS_FILEAPI
#define HTTPS_DESCRIPTIONS_FILEAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>

using namespace elevation;
class FileManagementApi {
public:
    FileManagementApi(Pistache::Rest::Description& desc, Logger& logger);

private:
    void getSuperviseurFile_       ( const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void deleteSuperviseurChanson_ ( const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void postSuperviseurInversion_ ( const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);

    Logger& m_logger;
};
#endif // !HTTPS_DESCRIPTIONS_FILEAPI

