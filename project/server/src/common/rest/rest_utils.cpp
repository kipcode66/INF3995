#include "rest_utils.hpp"

#include <iomanip>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

#include "database/Database.hpp"
#include "mp3/header/Mp3Duration.hpp"

namespace elevation {
namespace rest_utils {

std::string generateAllSongsAsViewedBy_(uint32_t token, bool adminSerialization) {
    std::vector<Song_t> songs = Database::instance()->getAllPlayableSongs();
    rapidjson::Document songsDoc;
    songsDoc.SetObject();
    rapidjson::Value songsArray;
    songsArray.SetArray();
    for (auto& song : songs) {
        songsArray.PushBack(generateSong_(song, token, songsDoc.GetAllocator(), adminSerialization), songsDoc.GetAllocator());
    }
    songsDoc.AddMember(rapidjson::Value().SetString("chansons"), songsArray, songsDoc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    songsDoc.Accept(writer);

    return buf.GetString();
}

rapidjson::Value& generateSong_(const Song_t& song, uint32_t token, rapidjson::Document::AllocatorType& allocator, bool adminSerialization) {
    rapidjson::Value songDoc;
    songDoc.SetObject();
    User_t user = Database::instance()->getUserById(song.userId);
    Mp3Duration d(song.duration);
    std::ostringstream duration;
    duration << d;
    songDoc.AddMember(rapidjson::StringRef("titre"), rapidjson::Value(song.title, strlen(song.title), allocator), allocator);
    songDoc.AddMember(rapidjson::StringRef("artiste"), rapidjson::Value(song.artist, strlen(song.artist), allocator), allocator);
    songDoc.AddMember("duree", rapidjson::Value(duration.str().c_str(), duration.str().length(), allocator), allocator);
    songDoc.AddMember("proposeePar", rapidjson::Value(user.name, strlen(user.name), allocator), allocator);
    songDoc.AddMember("proprietaire", token == song.userId ? true : false, allocator);
    songDoc.AddMember("no", song.id, allocator);

    if (adminSerialization) {
        Database* db = Database::instance();
        User_t owner = db->getUserById(song.userId);
        songDoc.AddMember("mac", rapidjson::Value(owner.mac, strlen(owner.mac), allocator), allocator);
        songDoc.AddMember("ip" , rapidjson::Value(owner.ip , strlen(owner.ip ), allocator), allocator);
        songDoc.AddMember("id" , owner.userId, allocator);
    }
    return songDoc.Move();
}

} // namespace rest_utils
} // namespace elevation
