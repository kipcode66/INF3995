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

std::string generateUser_(const User_t& user) {
    rapidjson::Document userDoc;
    userDoc.SetObject();
    userDoc.AddMember(rapidjson::StringRef("ip"), rapidjson::Value(user.ip, strlen(user.ip)), userDoc.GetAllocator());
    userDoc.AddMember(rapidjson::StringRef("mac"), rapidjson::Value(user.mac, strlen(user.mac)), userDoc.GetAllocator());
    userDoc.AddMember(rapidjson::StringRef("nom"), rapidjson::Value(user.name, strlen(user.name)), userDoc.GetAllocator());
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    userDoc.Accept(writer);

    return buf.GetString();
}

std::string generateStatistics_(const Statistics& stats) {
    rapidjson::Document statsDoc;
    statsDoc.SetObject();
    Mp3Duration songDuration(stats.getAverageDuration());
    std::stringstream duration;
    duration << songDuration;
    statsDoc.AddMember(rapidjson::StringRef("chansons"), rapidjson::Value(stats.getSongCount()), statsDoc.GetAllocator());
    statsDoc.AddMember(rapidjson::StringRef("utilisateurs"), rapidjson::Value(stats.getUserCount()), statsDoc.GetAllocator());
    statsDoc.AddMember(rapidjson::StringRef("elimines"), rapidjson::Value(stats.getDeletedSongsCount()), statsDoc.GetAllocator());
    statsDoc.AddMember(rapidjson::StringRef("temps"), rapidjson::Value(duration.str().c_str(), duration.str().length()), statsDoc.GetAllocator());
    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    statsDoc.Accept(writer);

    return buf.GetString();
}

std::string generateBody_(uint32_t id , std::string message) {
    rapidjson::Document idDoc;
    idDoc.SetObject();
    idDoc.AddMember("identificateur", id, idDoc.GetAllocator());
    idDoc.AddMember("message", rapidjson::Value(message.c_str(), message.length()), idDoc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    idDoc.Accept(writer);
    return buf.GetString();
}


} // namespace rest_utils
} // namespace elevation
