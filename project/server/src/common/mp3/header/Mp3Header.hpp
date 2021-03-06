#ifndef SRC_HTTP_MP3_HEADER_HPP
#define SRC_HTTP_MP3_HEADER_HPP

#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/mpegfile.h>
#include <taglib/mpegproperties.h>
#include <iostream>
#include <string>

#include "Mp3Duration.hpp"

class Mp3Header {
public:
    explicit Mp3Header(const std::string& fileName);
    virtual ~Mp3Header();

    Mp3Duration getDuration();
    void displaySongInfo();
    std::string getTitle();
    std::string getArtist();
    std::string getAlbum();
    std::string getGenre();
    int getYear();
    int getTrack();
    int getSampleRate();

private:
    void getSongInfo_(TagLib::Tag* mp3Tag, TagLib::MPEG::Properties* mp3Properties);
    bool isMP3File_(const std::string& fileName);

    Mp3Duration m_duration;
    std::string m_title;
    std::string m_artist;
    std::string m_album;
    std::string m_genre;
    int m_year;
    int m_track;
    int m_sampleRate;
};

#endif // !SRC_HTTP_MP3_HEADER_HPP
