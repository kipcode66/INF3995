#ifndef MP3_PLAYER_PENDINGSONGS_HPP
#define MP3_PLAYER_PENDINGSONGS_HPP

#include <experimental/filesystem>
#include <list>
#include <mutex>
#include <thread>
#include <memory>
#include <future>

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
    PendingSongs(PendingSongs&&) = delete; ///< Cannot move() because we own a thread, whose 'this' pointer isn't updated when we move.
    virtual ~PendingSongs();

    PendingSongs& operator= (const PendingSongs&) = delete;
    PendingSongs& operator= (PendingSongs&&) = delete; ///< Cannot move() because we own a thread, whose 'this' pointer isn't updated when we move.

    void addSong     (const std::experimental::filesystem::path& songPath);
    void removeSong  (const std::experimental::filesystem::path& songPath);
    void reorderSongs(std::size_t songAPosition, std::size_t songBPosition);
    std::list<std::experimental::filesystem::path> getPendingSongs() const { ///< @note This method is not thread-safe.
        return m_pendingSongs;
    }

protected:
    /**
     * @brief Thread method which starts the songs on the #Mp3Player.
     * Asynchronously waits until a song is added to start playing,
     * and waits until the song finishes.
     */
    void songStarter_();
    void stopSong_();

protected:
    static std::future<void> defaultFuture_();

protected:
    Mp3Player m_player;
    std::list<std::experimental::filesystem::path> m_pendingSongs;
    std::size_t m_maxSongs;
    std::mutex m_songListMutex;
    std::thread m_playerThread;
    std::future<void> m_start;
    std::future<void> m_stop;
    std::atomic<bool> m_terminateRequested;
};

} // namespace elevation

#endif // !MP3_PLAYER_PENDINGSONGS_HPP
