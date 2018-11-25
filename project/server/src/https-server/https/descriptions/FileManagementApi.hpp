#ifndef HTTPS_DESCRIPTIONS_FILEAPI
#define HTTPS_DESCRIPTIONS_FILEAPI

#include <pistache/description.h>
#include <common/logger/Logger.hpp>
#include <common/filesystem/FileCache.hpp>


class FileManagementApi {
public:
    FileManagementApi(Pistache::Rest::Description&,
                      elevation::Logger& logger,
                      elevation::FileCache&);

private:
    void getSuperviseurFile_       (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void deleteSuperviseurChanson_ (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);
    void postSuperviseurInversion_ (const Pistache::Rest::Request&, Pistache::Http::ResponseWriter);

    elevation::Logger& m_logger;
    elevation::FileCache& m_cache;
};
#endif // !HTTPS_DESCRIPTIONS_FILEAPI

