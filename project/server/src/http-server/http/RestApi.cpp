#include "RestApi.hpp"
#include "database/Database.hpp"
#include "misc/Base64.hpp"
#include <thread>
#include <future>
#include <ctime>
#include <iomanip>
#include "mp3/header/Mp3Header.hpp"

using namespace elevation;

RestApi::RestApi(Address addr)
: m_httpEndpoint(std::make_shared<Http::Endpoint>(addr))
, m_desc("Rest API", "1.0")
{ }

RestApi::~RestApi() {
    m_httpEndpoint->shutdown();
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
            .route(m_desc.get("/identification"))
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

void buildUserFromQuery_(struct User* __restrict__ newUser,
                         Pistache::Http::Uri::Query* __restrict__ query) {
    strcpy(newUser->mac, query->get("mac").get().c_str());
    strcpy(newUser->ip, query->get("ip").get().c_str());
    strcpy(newUser->name, query->get("name").get().c_str());
}

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {
    puts("getIdentification function called");
    std::async([&](){
        auto token = request.headers().getRaw("X-Auth-Token");
        uint32_t t = std::stoi(token.value());

        auto query = request.query();
        if (!query.has("mac")) {
            response.send(Http::Code::Bad_Request, "Malformed request");
        } else {
            std::string mac(query.get("mac").get());

            struct User newUser = { 0 };
            struct User oldUser = { 0 };

            Database* db = Database::instance();
            db->getUserByMac(mac.c_str(), &oldUser);
            if (*oldUser.mac == 0) {
                buildUserFromQuery_(&newUser, &query);
                db->createUser(&newUser);
                response.send(Http::Code::Ok, "{\"identificateur\": " + std::to_string(t) + ", \"message\":\"Bienvenue au café-bistro Élévation!\"}");
            } else {
                buildUserFromQuery_(&newUser, &query);
                newUser.id = oldUser.id;
                if (db->createUser(&newUser)) {
                    response.send(Http::Code::Internal_Server_Error, "couldn't create user in db");
                } else {
                    response.send(Http::Code::Ok, "{\"identificateur\": " + std::to_string(t) + ", \"message\":\"Bienvenue au cafe-bistro Elevation!\"}");
                }
            }
            response.send(Http::Code::Ok, "getIdentification called");
        }
    });
    return;
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::async([&](){
        std::string param = request.param(":id").as<std::string>();
        response.send(Http::Code::Ok, "{\n\"chansons\":[\n"
            "{\n\"titre\":\"Never Gonna Give You Up\",\n\"artiste\":\"Foo\",\n\"duree\":\"4:20\",\n\"proposeePar\":\"Chuck Norris\",\n\"proprietaire\":false,\n\"no\":42},\n"
            "{\n\"titre\":\"Hey Jude\",\n\"artiste\":\"Beatles\",\n\"duree\":\"7:05\",\n\"proposeePar\":\"Claude\",\n\"proprietaire\":true,\n\"no\":25}\n"
        "]\n}\n");
        std::cout << "getFileList function called, param is " << param << std::endl;
    });
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    std::cout << "postFile function called" << std::endl;

    auto t = request.headers().getRaw("X-Auth-Token");
    if (t.value().empty()) {
        response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" missing");
        return;
    }

    if (!m_cache.isInitialized()) {
        response.send(Http::Code::Internal_Server_Error, "Cache not initialized");
        return;
    }

    std::async([&](){
        try {
            uint32_t token = std::stoi(t.value());

            // TODO : Check if the user has a valid token.

            // Decode the string.
            std::stringstream encoded(request.body());
            std::stringstream decoded;
            Base64::Decode(encoded, decoded);

            // +=-=-=-= Save in a file =-=-=-=+
            // Generate
            std::time_t now = std::time(nullptr);
            std::tm* nowTm = std::localtime(&now);
            std::stringstream fileName;
            // Generate a new name 
            fileName << std::put_time(nowTm, "%Y-%m-%d_%H-%M-%S") << std::hash<std::string>()(decoded.str());
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

            // TODO : Put the song's path in the DB.

            response.send(Http::Code::Ok, "Ok"); // We send the response at the end in the case there is an error in the process.

            delete header;
            return;
        }
        catch (std::logic_error &e) {
            std::cout << "Token not valid; got \"" << t.value() << "\"" << std::endl;
            response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" must be a 32 bits integer");
            return;
        }
        response.send(Http::Code::Internal_Server_Error, "Request terminated without an answer...");
    });
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    std::cout << "deleteFile function called" << std::endl;
}
