#include <stdint.h>
#include <cstddef>

struct Song_t {
    static constexpr const size_t TITLE_LENGTH = 256;
    static constexpr const size_t ARTIST_LENGTH = 256;
    static constexpr const size_t PATH_LENGTH = 256;

    uint32_t id;
    char title[TITLE_LENGTH];
    char artist[ARTIST_LENGTH];
    uint32_t user_id;
    char path[PATH_LENGTH];
};