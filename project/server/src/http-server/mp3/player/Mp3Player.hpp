#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>
#include <thread>
#include <condition_variable>

namespace elevation {

class Mp3Player {
public:
    explicit Mp3Player();
    virtual ~Mp3Player();

    void startPlaying(const std::string& fileName);
    void waitUntilSongFinished();

protected:
    void run_(std::string fileName);
    bool isSongPlaying_();
    void waitUntilSongStarted_();
    void setIsPlayingTo_(bool value);

private:
    std::thread m_playerThread;
    bool m_isPlaying;
    std::condition_variable m_isPlayingCondition;
    std::mutex m_isPlayingMutex;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
