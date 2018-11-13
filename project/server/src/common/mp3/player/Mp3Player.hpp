#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>
#include <thread>
#include <future>
#include <atomic>
#include <memory>

namespace elevation {

class Mp3Player {
protected:
    static void run_(std::string fileName, std::shared_ptr<std::atomic<bool>> stopRequested);

public:
    explicit Mp3Player();
    Mp3Player(const Mp3Player&) = delete;
    Mp3Player(Mp3Player&& that);

    virtual ~Mp3Player();

    Mp3Player& operator=(const Mp3Player&) = delete;
    Mp3Player& operator=(Mp3Player&& that);

    void startPlaying(const std::string& fileName);
    void stopPlaying();
    void waitUntilSongFinished();

protected:
    static std::future<void> defaultFuture_();

private:
    std::future<void> m_player;
    std::unique_ptr<std::mutex> m_playerMutex;
    std::shared_ptr<std::atomic<bool>> m_stopRequested;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
