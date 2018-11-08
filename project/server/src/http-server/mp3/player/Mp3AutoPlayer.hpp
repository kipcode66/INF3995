#ifndef MP3_PLAYER_MP3AUTOPLAYER_HPP
#define MP3_PLAYER_MP3AUTOPLAYER_HPP

#include <experimental/filesystem>
#include <list>
#include <mutex>
#include <thread>
#include <memory>
#include <future>
#include <chrono>

#include "mp3/player/Mp3Player.hpp"

namespace elevation {

/**
 * @brief A class which plays songs on its own.
 * It automagically starts playing the next song when we add it, and plays the next
 * one when the first one is finished, and so forth.
 *
 * @note Unlike #Mp3Player, destruction does (should) not wait until
 * the song is finished playing ; it should just stop the current song.
 */
class Mp3AutoPlayer {
protected:
    typedef ::std::experimental::filesystem::path path;

protected:
    static constexpr std::chrono::seconds NEXT_SONG_POLLING_DELAY{1};

public:
    /**
     * @brief Constructor.
     * @param[in] nextSongGetter Callback which will be called when a new song should be started.
     * Will be called periodically until the return value is not an empty path ("").
     * @param[in] songRemover Callback which will be called when a song finishes and needs to be removed
     * from the song list.
     */
    explicit Mp3AutoPlayer(
        std::function<path()> nextSongGetter,
        std::function<void(path)> songRemover
    );
    Mp3AutoPlayer(const Mp3AutoPlayer&) = delete;
    Mp3AutoPlayer(Mp3AutoPlayer&&) = delete; ///< Cannot move() because we own a thread, whose 'this' pointer isn't updated when we move.
    virtual ~Mp3AutoPlayer();

    Mp3AutoPlayer& operator=(const Mp3AutoPlayer&) = delete;
    Mp3AutoPlayer& operator=(Mp3AutoPlayer&&) = delete; ///< Cannot move() because we own a thread, whose 'this' pointer isn't updated when we move.

    /**
     * @brief Stops the song that is currently playing. If there is none,
     * this method does nothing.
     */
    void stopSong();

    /**
     * @brief Useful for unit tests, but can be used in production too.
     */
    void waitUntilSongStarted();

    /**
     * @brief Useful for unit tests, but can be used in production too.
     */
    void waitUntilSongFinished();

protected:
    /**
     * @brief Thread method which starts the songs on the #Mp3Player.
     * Asynchronously waits until a song is added to start playing,
     * and waits until the song finishes.
     */
    void songStarter_();
    void sendTerminate_();
    void sendStartSignal();

protected:
    Mp3Player m_player;
    std::thread m_playerThread;
    std::promise<void> m_startPromise;
    std::shared_future<void> m_startFuture;
    std::mutex m_startMutex;

    std::function<std::experimental::filesystem::path()> m_nextSongGetter;
    std::function<void(std::experimental::filesystem::path)> m_songRemover;

    std::atomic<bool> m_terminateRequested;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3AUTOPLAYER_HPP
