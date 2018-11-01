#ifndef MP3_PLAYER_PENDINGSONGS_HPP
#define MP3_PLAYER_PENDINGSONGS_HPP

#include <experimental/filesystem>
#include <list>
#include <mutex>

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
    explicit PendingSongs(std::size_t maxSongs);
    virtual ~PendingSongs();

    void addSong    (const std::string& songName);
    void removeSong (const std::string& songName);
    void reorderSong(std::size_t songAPosition, std::size_t songBPosition);

protected:
    Mp3Player m_player;
    std::list<std::experimental::filesystem::path> m_pendingSongs;
    std::size_t m_maxSongs;
    std::mutex m_mutex;
};

} // namespace elevation

#endif // !MP3_PLAYER_PENDINGSONGS_HPP
