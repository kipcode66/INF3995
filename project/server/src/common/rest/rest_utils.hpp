#ifndef REST_RESTUTILS_HPP
#define REST_RESTUTILS_HPP

#include <rapidjson/document.h>

#include "database/templates/Song.hpp"

namespace elevation {
namespace rest_utils {

std::string generateAllSongsAsViewedBy_(uint32_t token, bool adminSerialization = false);
rapidjson::Value& generateSong_(const Song_t& song, uint32_t token, rapidjson::Document::AllocatorType& allocator, bool adminSerialization);

} // namespace rest_utils
} // namespace elevation

#endif // !REST_RESTUTILS_HPP
