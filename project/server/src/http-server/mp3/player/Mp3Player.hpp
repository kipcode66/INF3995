#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>
#include <thread>

namespace elevation {

class Mp3Player {
protected:
    static void run_(std::string fileName);

public:
    explicit Mp3Player();
    virtual ~Mp3Player();

    void startPlaying(const std::string& fileName);

private:
    std::thread m_playing;
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
