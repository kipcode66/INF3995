#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>
#include <future>

#include "SecureRestApi.hpp"
#include "misc/id_utils.hpp"
#include "rapidjson/document.h"

#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr, Logger& logger, FileCache& cache, Mp3EventClientSocket playerEventSocket)
    : RestApi(addr, logger, cache, std::move(playerEventSocket))
    , m_logger            (logger)
    , m_volumeApi         (m_desc)
    , m_blacklistApi      (m_desc, m_logger)
    , m_statsApi          (m_desc)
    , m_fileManagementApi (m_desc, m_logger)
    , m_authApi           (m_desc, m_logger)
{ }

void SecureRestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
}

