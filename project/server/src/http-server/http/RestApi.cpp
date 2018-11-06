#include <math.h>
#include <thread>
#include <future>
#include <ctime>
#include <iomanip>

#include "RestApi.hpp"
#include "database/Database.hpp"
#include "misc/Base64.hpp"
#include "mp3/header/Mp3Header.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>

#include <sstream>

using namespace elevation;

RestApi::RestApi(Address addr)
: m_httpEndpoint(std::make_shared<Http::Endpoint>(addr))
, m_desc("Rest API", "1.0")
, m_logger(Logger::getLogger("http-server"))
{
    Database::instance();
}

RestApi::~RestApi() {
    try {
        m_httpEndpoint->shutdown();
    }
    catch (std::runtime_error& e) { } // Pistache seems to have an issue where calling shutdown throws an exception.
}

void RestApi::init() {
    auto opts = Http::Endpoint::options();
    m_httpEndpoint->init(opts);
    createDescription_();
}

void RestApi::start() {
    m_router.initFromDescription(m_desc);

    Rest::Swagger swagger(m_desc);
    swagger
            .install(m_router);

    m_httpEndpoint->setHandler(m_router.handler());
    m_httpEndpoint->serve();
}

void RestApi::createDescription_() {
    m_desc
            .info()
            .license("Apache", "http://www.apache.org/licenses/LICENSE-2.0");

    auto backendErrorResponse =
            m_desc.response(Http::Code::Internal_Server_Error, "An error occurred with the backend");

    m_desc
            .schemes(Rest::Scheme::Http)
            .produces(MIME(Application, Json))
            .consumes(MIME(Application, Json));

    auto usagerPath = m_desc.path("/usager");

    usagerPath
            .route(m_desc.post("/identification"))
            .bind(&RestApi::getIdentification_, this)
            .hide();

    usagerPath
            .route(m_desc.get("/file/:id"))
            .bind(&RestApi::getFileList_, this)
            .hide();

    usagerPath
            .route(m_desc.post("/chanson/:id"))
            .bind(&RestApi::postFile_, this)
            .hide();

    usagerPath
            .route(m_desc.del("/chanson/:id/:no"))
            .bind(&RestApi::deleteFile_, this)
            .hide();
}


std::string generateBody(uint32_t id, std::string message) {
    rapidjson::Document idDoc;
    idDoc.SetObject();
    idDoc.AddMember("identificateur", id, idDoc.GetAllocator());
    idDoc.AddMember("message", rapidjson::Value(message.c_str(), message.length()), idDoc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    idDoc.Accept(writer);
    return buf.GetString();
}

std::string generateSong(const Song_t& song, uint32_t token) {
    rapidjson::Document songDoc;
    songDoc.SetObject();
    songDoc.AddMember(rapidjson::StringRef("titre"), rapidjson::Value(song.title, strlen(song.title)), songDoc.GetAllocator());
    songDoc.AddMember(rapidjson::StringRef("artiste"), rapidjson::Value(song.artist, strlen(song.artist)), songDoc.GetAllocator());
    songDoc.AddMember("duree", "04:20", songDoc.GetAllocator());
    User_t user;
    Database::instance()->getUserById(token, &user);
    songDoc.AddMember("proposeePar", rapidjson::Value(user.name, strlen(user.name)), songDoc.GetAllocator());
    songDoc.AddMember("proprietaire", token == song.user_id ? true : false, songDoc.GetAllocator());
    songDoc.AddMember("no", song.id, songDoc.GetAllocator());

    rapidjson::StringBuffer buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
    songDoc.Accept(writer);

    return buf.GetString();
}

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {
    std::string body = request.body();
    rapidjson::Document request_json;
    request_json.Parse(body.c_str());

    std::cout << "getIdentification called: body [" << body << "]; " << std::endl;

    if ((request_json.IsObject()
            && (!request_json.HasMember("mac")
                || !request_json.HasMember("ip")
                || request_json["mac"] == '\n'
                || request_json["ip"] == '\n'))
            || ( !request_json.IsObject() && !(
                request.hasParam("mac") && request.hasParam("ip")
            ))) {
        response.send(Http::Code::Bad_Request, "Malformed request");
        return;
    }
    std::async([&](){
        User_t requestUser = { 0 };
        if (request_json.IsObject()) {
            strcpy(requestUser.mac, request_json["mac"].GetString());
            strcpy(requestUser.ip, request_json["ip"].GetString());
            strcpy(requestUser.name, request_json["nom"].GetString());
        }
        else if (request.hasParam("mac") && request.hasParam("ip")) {
            strcpy(requestUser.mac, request.param("mac").as<std::string>().c_str());
            strcpy(requestUser.ip, request.param("ip").as<std::string>().c_str());
            if (request.hasParam("nom")) {
                strcpy(requestUser.name, request.param("nom").as<std::string>().c_str());
            }
        }

        std::ostringstream osStream;
        osStream << '{' << requestUser.mac << '}' << " Assigned ID \"" << "ID TODO" << "\" to user \"" << requestUser.name;
        m_logger.log(osStream.str());

        User_t existingUser = { 0 };
        Database* db = Database::instance();
        db->getUserByMac(requestUser.mac, &existingUser);
        if (*existingUser.mac == 0) {
            db->createUser(&requestUser);
            db->getUserByMac(requestUser.mac, &existingUser);

            // MOCK id TODO generate and insert in db
            uint32_t id = existingUser.id;
            std::string body = generateBody(id, "connection successful");
            response.send(Http::Code::Ok, body);
        } else {
            requestUser.id = existingUser.id;
            if (db->createUser(&requestUser)) {
                std::cerr << "problem writing to database" << std::endl;
                response.send(Http::Code::Internal_Server_Error, "couldn't create user in db");
            } else {
                db->getUserByMac(requestUser.mac, &existingUser);
                uint32_t id = existingUser.id;
                std::string body = generateBody(id, "connection successful");
                response.send(Http::Code::Ok, body);
            }
        }
    });

    // response.send(Http::Code::Ok, "getIdentification called");
    return;
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::string param = request.param(":id").as<std::string>();
    std::async([&](){
        std::cout << "getFileList function called, param is " << param << std::endl;
        if (std::stoi(param) == 0) {
            response.send(Http::Code::Forbidden, "Invalid token");
            return;
        }
        Database* db = Database::instance();
        std::vector<Song_t> songs = db->getAllSongs();
        std::stringstream resp;
        resp << "{\n\"chansons\":[\n";
        for (auto& song : songs) {
            resp << generateSong(song, std::stoi(param)) << (&songs.back() != &song ? ",\n" : "\n");
        }
        resp << "]\n}\n";
        response.send(Http::Code::Ok, resp.str());
        /*"{\n\"chansons\":[\n"
            "{\n\"titre\":\"Never Gonna Give You Up\",\n\"artiste\":\"Foo\",\n\"duree\":\"4:20\",\n\"proposeePar\":\"Chuck Norris\",\n\"proprietaire\":false,\n\"no\":42},\n"
            "{\n\"titre\":\"Hey Jude\",\n\"artiste\":\"Beatles\",\n\"duree\":\"7:05\",\n\"proposeePar\":\"Claude\",\n\"proprietaire\":true,\n\"no\":25}\n"
        "]\n}\n"*/
    });
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    std::cout << "postFile function called" << std::endl;

    User_t requestUser; // TODO
    std::ostringstream osStream;
    osStream << '{' << requestUser.mac << '}' << " Sent MP3 \"" << "TITLE TODO" << "\" of length " << "SONG LENGTH TODO";
    m_logger.log(osStream.str());

    auto t = request.headers().getRaw("X-Auth-Token");
    if (t.value().empty()) {
        std::cout << "Header \"X-Auth-Token\" missing" << std::endl;
        response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" missing");
        return;
    }

    if (!m_cache.isInitialized()) {
        std::cout << "Cache not initialized" << std::endl;
        response.send(Http::Code::Internal_Server_Error, "Cache not initialized");
        return;
    }

    std::string body = request.body();

    std::async([=, &response](){
        try {
            uint32_t token = std::stoi(t.value());

            // TODO : Check in the DB if the user has a valid token.
            if (token == 0) {
                response.send(Http::Code::Forbidden, "Invalid token");
                return;
            }

            // Decode the string.
            std::stringstream encoded(body);
            std::stringstream decoded;
            Base64::Decode(encoded, decoded);

            // +=-=-=-= Save in a file =-=-=-=+
            // Generate
            std::time_t now = std::time(nullptr);
            std::tm* nowTm = std::localtime(&now);
            std::stringstream fileName;
            // Generate a new name 
            fileName << std::put_time(nowTm, "%Y-%m-%d_%H-%M-%S_") << std::hash<std::string>()(decoded.str());
            std::experimental::filesystem::path filePath(fileName.str());
            std::experimental::filesystem::path tmpPath = filePath;
            // Try a new file name until we find on that is not used.
            size_t counter = 0;
            while (m_cache.isFileCached(tmpPath)) {
                tmpPath = filePath;
                tmpPath += "_" + std::to_string(++counter);
            }
            tmpPath += ".mp3";
            filePath = tmpPath;

            m_cache.setFileContent(filePath, decoded);
            filePath = m_cache.getFile(filePath).path(); // resolve the filename into a real path

            std::cout << "decoded : " << decoded.str().substr(0, 30) << std::endl;

            // Fetch MP3 header
            Mp3Header* header;
            try {
                header = new Mp3Header(filePath.string());
            }
            catch (std::invalid_argument& e) {
                std::cout << "It's NOT tag id3v2 ..." << std::endl;
                response.send(Http::Code::Unsupported_Media_Type, "The file is not an MP3 file");
                m_cache.deleteFile(filePath.filename());
                return;
            }

            std::cout << "It's tag id3v2 !!!" << std::endl;
            std::cout << "Title: " << header->getTitle() << std::endl;

            // Put the song's path in the DB.
            Song_t song;
            strcpy(song.title, header->getTitle().c_str());
            strcpy(song.artist, header->getArtist().c_str());
            song.user_id = token; // TODO Get the user from the token and get the id from the user.
            strcpy(song.path, filePath.string().c_str());

            Database* db = Database::instance();

            const auto& songs = db->getSongsByUser(token);
            bool songInQueue = std::any_of(songs.cbegin(), songs.cend(), [&](const Song_t& a) {return strcmp(a.title, song.title) == 0;});
            if (songInQueue || songs.size() >= MAX_SONG_PER_USER) {
                response.send(Http::Code::Request_Entity_Too_Large, "Song already in the queue");
                m_cache.deleteFile(filePath.filename());
                return;
            }

            db->createSong(&song);

            response.send(Http::Code::Ok, "Ok"); // We send the response at the end in the case there is an error in the process.

            delete header;
            return;
        }
        catch (std::logic_error &e) {
            std::cout << "Token not valid; got \"" << t.value() << "\"" << std::endl;
            response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" must be a 32 bits integer");
            return;
        }
        catch (std::runtime_error &e) {
            std::ostringstream osStream;
            osStream << "An error occured while saving the song : " << e.what();
            m_logger.log(osStream.str());
            std::cerr << osStream.str();
            response.send(Http::Code::Internal_Server_Error, e.what());
            return;
        }
        response.send(Http::Code::Internal_Server_Error, "Request terminated without an answer...");
    });
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");

    User_t requestUser; // TODO
    std::ostringstream osStream;
    osStream << '{' << requestUser.mac << '}' << " Removed MP3 \"" << "TITLE TODO" << "\" of length " << "SONG LENGTH TODO";
    m_logger.log(osStream.str());

    std::cout << "deleteFile function called" << std::endl;
}
