#ifndef MP3_PLAYER_MP3PLAYER_HPP
#define MP3_PLAYER_MP3PLAYER_HPP

#include <string>

namespace elevation {

class Mp3Player {
protected:
    static void run_(const std::string& fileName);

public:
    explicit Mp3Player();
    virtual ~Mp3Player();

    void startPlaying(const std::string& fileName);
};

} // namespace elevation

#endif // !MP3_PLAYER_MP3PLAYER_HPP
