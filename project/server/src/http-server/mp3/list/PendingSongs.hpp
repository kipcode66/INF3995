#ifndef MP3_PLAYER_PENDINGSONGS_HPP
#define MP3_PLAYER_PENDINGSONGS_HPP

#include "mp3/player/Mp3Player.hpp"

namespace elevation {

/**
 * @brief A class to which we simply add or remove songs.
 * It automagically starts playing a song when we add one, and plays the next
 * one when the first one is finished.
 */
class PendingSongs {
public:
    explicit PendingSongs();
    virtual ~PendingSongs();

    void addSong    (/* TODO */);
    void removeSong (/* TODO */);
    void reorderSong(/* TODO */);

protected:
    Mp3Player m_player;
};

} // namespace elevation

#endif // !MP3_PLAYER_PENDINGSONGS_HPP
