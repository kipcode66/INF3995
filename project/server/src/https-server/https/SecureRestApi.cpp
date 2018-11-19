#include <common/logger/Logger.hpp>
#include <pistache/serializer/rapidjson.h>
#include <future>

#include "SecureRestApi.hpp"
#include "rapidjson/document.h"
#include "misc/id_utils.hpp"
#include "database/Database.hpp"

#include "rapidjson/document.h"

#include <common/database/Database.hpp>

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr, Logger& logger, FileCache& cache)
: RestApi(addr, logger, cache)
, m_volumeApi(m_desc)
, m_blacklistApi(m_desc)
, m_statsApi(m_desc)
, m_logger(logger)
{ }

void SecureRestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    /* createDescription_(); */
    /* createSecureDescription_(); */
}

