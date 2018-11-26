#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>
#include <future>

#include "SecureRestApi.hpp"
#include "misc/id_utils.hpp"
#include "rapidjson/document.h"

#include <common/database/Database.hpp>
#include <common/logger/Logger.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Pistache::Address addr, Logger& logger, FileCache& cache, Mp3EventClientSocket playerEventSocket)
    : RestApi(addr, logger, cache)
    , m_logger            (logger)
    , m_socket(new Mp3EventClientSocket(std::move(playerEventSocket)))
    , m_eventFacade(logger, m_socket)
    , m_volumeApi         (m_desc, m_logger, m_socket, m_eventFacade)
    , m_blacklistApi      (m_desc, m_logger)
    , m_statsApi          (m_desc, m_logger)
    , m_fileManagementApi (m_desc, m_logger, m_cache)
    , m_authApi           (m_desc, m_logger)
{ }

void SecureRestApi::init() {
    auto opts = Pistache::Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
}

