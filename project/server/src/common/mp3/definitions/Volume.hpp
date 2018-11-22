#ifndef MP3_DEFINITIONS_VOLUME_HPP
#define MP3_DEFINITIONS_VOLUME_HPP

#include <inttypes.h>

namespace elevation {

/**
 * @brief Volume level.
 * 0 -> No sound
 * >= 100 -> Max
 */
typedef uint8_t volumePercent_t;

} // namespace elevation

#endif // !MP3_DEFINITIONS_VOLUME_HPP
