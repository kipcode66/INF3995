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
, m_logger(logger)
{ }

void SecureRestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
    createSecureDescription_();
}

void SecureRestApi::createSecureDescription_() {

    /**
     * File Management
     */
    createFileManagementDescription_(m_desc);

    /**
     * Volume Management
     */
    /* createVolumeDescription_(m_desc); */

    /**
     * Statistics
     */
    createStatisticsDescription_(m_desc);

    /**
     * Blacklist management
     */
    createBlacklistDescription_(m_desc);

    /**
     * Login Management
     */
    createAuthDescription_(m_desc);

    /* this->m_volumeApi = new VolumeApi(this->m_desc); */
}

