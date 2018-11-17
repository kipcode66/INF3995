#ifndef MP3_MP3AUTOPLAYER_CALLBACKS_HPP
#define MP3_MP3AUTOPLAYER_CALLBACKS_HPP

#include <mp3/player/Mp3AutoPlayer.hpp>
#include <logger/Logger.hpp>
#include <filesystem/FileCache.hpp>

namespace elevation {

class Mp3AutoPlayerCallbacks {
protected:
    typedef ::std::experimental::filesystem::path path;

public:
    Mp3AutoPlayerCallbacks(Logger& logger, FileCache& cache);
    Mp3AutoPlayerCallbacks(const Mp3AutoPlayerCallbacks&) = delete;
    Mp3AutoPlayerCallbacks(Mp3AutoPlayerCallbacks&&) = delete; // See Mp3AutoPlayer.hpp
    virtual ~Mp3AutoPlayerCallbacks();

    Mp3AutoPlayerCallbacks& operator=(const Mp3AutoPlayerCallbacks&) = delete;
    Mp3AutoPlayerCallbacks& operator=(Mp3AutoPlayerCallbacks&&) = delete; // See Mp3AutoPlayer.hpp

    Mp3AutoPlayer& getReferenceToAutoPlayer() const;

protected:
    Logger& m_logger;
    FileCache& m_cache;
    Mp3AutoPlayer* m_autoPlayer;

    path newSongProvider_() const;
    void songRemover_(path);

};

}

#endif // !MP3_MP3AUTOPLAYER_CALLBACKS_HPP
