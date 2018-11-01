#ifndef MP3_PLAYER_PENDINGSONGS_HPP
#define MP3_PLAYER_PENDINGSONGS_HPP

#include <experimental/filesystem>

#include "mp3/player/Mp3Player.hpp"

namespace elevation {

/**
 * @brief A class to which we simply add or remove songs.
 * It automagically starts playing a song when we add one, and plays the next
 * one when the first one is finished.
 */
class PendingSongs {
public:
    static const std::experimental::filesystem::path SONGS_DIR; ///< @todo Mock value ; remove
public:
    explicit PendingSongs();
    virtual ~PendingSongs();

    void addSong    (const std::string& songName);
    void removeSong (const std::string& songName);
    void reorderSong(const std::string& songName, std::size_t newPosition);

protected:
    Mp3Player m_player;
};

} // namespace elevation

#endif // !MP3_PLAYER_PENDINGSONGS_HPP
