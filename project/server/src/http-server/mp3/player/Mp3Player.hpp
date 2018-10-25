#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>
#include <thread>
#include <future>

namespace elevation {

class Mp3Player {
public:
    explicit Mp3Player();
    Mp3Player(const Mp3Player&) = delete;
    Mp3Player(Mp3Player&& that);

    virtual ~Mp3Player();

    Mp3Player& operator=(const Mp3Player&) = delete;
    Mp3Player& operator=(Mp3Player&& that);

    void startPlaying(const std::string& fileName);
    void waitUntilSongFinished();

protected:
    void run_(std::string fileName);

protected:
    static std::future<void> defautFuture_();

private:
    std::future<void> m_player;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
