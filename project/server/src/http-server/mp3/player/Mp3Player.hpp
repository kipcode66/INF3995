#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>
#include <thread>
#include <future>

namespace elevation {

class Mp3Player {
public:
    explicit Mp3Player();
    virtual ~Mp3Player();

    void startPlaying(const std::string& fileName);
    void waitUntilSongFinished();

protected:
    bool run_(std::string fileName);

private:
    std::future<bool> m_player;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
