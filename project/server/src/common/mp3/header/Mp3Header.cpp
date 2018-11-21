#include "Mp3Header.hpp"
#include <stdexcept>

Mp3Header::Mp3Header(const std::string& fileName)
{
    TagLib::MPEG::File mp3File(fileName.c_str());
    if (!mp3File.isOpen() || !isMP3File_(fileName) || !mp3File.hasID3v2Tag()) {
        throw std::invalid_argument( "MP3 file failed to open" );
    }
    TagLib::ID3v2::Tag* mp3Tag = mp3File.ID3v2Tag(true);
    TagLib::MPEG::Properties* mp3Properties = mp3File.audioProperties();
    getSongInfo_(mp3Tag, mp3Properties);
}

Mp3Header::~Mp3Header()
{ }

void Mp3Header::displaySongInfo() {
    std::cout << " l'album est " << m_album << " l'artiste est " << m_artist << std::endl;
    std::cout << " le genre est " << m_genre << " le titre est " << m_title << std::endl;
    std::cout << " le numero de chanson est " << m_track << " la chanson a ete ecrite en " << m_year << std::endl;
    std::cout << " la chanson dure  " << m_duration.getMinutes() << " minutes et " << m_duration.getSeconds() << " secondes " << std::endl;
}

void Mp3Header::getSongInfo_(TagLib::ID3v2::Tag* mp3Tag, TagLib::MPEG::Properties* mp3Properties) {
    m_title = mp3Tag->title().toCString();
    m_artist = mp3Tag->artist().toCString();
    m_album = mp3Tag->album().toCString();
    m_genre = mp3Tag->genre().toCString();
    m_year = mp3Tag->year();
    m_track = mp3Tag->track();
    m_duration = Mp3Duration(mp3Properties->lengthInSeconds());
    m_sampleRate = mp3Properties->sampleRate();
}

bool Mp3Header::isMP3File_(const std::string& fileName) {
    if(fileName.substr(fileName.find_last_of(".") + 1) == "mp3")
        return true;
    return false;
}

Mp3Duration Mp3Header::getDuration() {
    return m_duration;
}

std::string Mp3Header::getTitle() {
    return m_title;
}

std::string Mp3Header::getArtist() {
    return m_artist;
}

std::string Mp3Header::getAlbum() {
    return m_album;
}

std::string Mp3Header::getGenre() {
    return m_genre;
}

int Mp3Header::getYear() {
    return m_year;
}

int Mp3Header::getTrack() {
    return m_track;
}

int Mp3Header::getSampleRate() {
    return m_sampleRate;
}
