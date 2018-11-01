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
    explicit PendingSongs(std::size_t maxSongs);
    PendingSongs(const PendingSongs&) = delete;
    PendingSongs(PendingSongs&&) = delete;
    virtual ~PendingSongs();

    PendingSongs& operator= (const PendingSongs&) = delete;
    PendingSongs& operator= (PendingSongs&&) = delete;

    void addSong     (const std::experimental::filesystem::path& songPath);
    void removeSong  (const std::experimental::filesystem::path& songPath);
    void reorderSongs(std::size_t songAPosition, std::size_t songBPosition);
    std::list<std::experimental::filesystem::path> getPendingSongs() const { ///< @note This method is not thread-safe.
        return m_pendingSongs;
    }

protected:
    Mp3Player m_player;
    std::list<std::experimental::filesystem::path> m_pendingSongs;
    std::size_t m_maxSongs;
    std::mutex m_mutex;
};

} // namespace elevation

#endif // !MP3_PLAYER_PENDINGSONGS_HPP
