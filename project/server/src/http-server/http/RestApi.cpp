#include "RestApi.hpp"
#include "database/Database.hpp"
#include "misc/Base64.hpp"
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>

using namespace elevation;

StringID3v2Tag::StringID3v2Tag(const std::string& data) {
    parse(TagLib::String(data).data(TagLib::String::Type::UTF8));
}

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

    return;
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::string param = request.param(":id").as<std::string>();
    response.send(Http::Code::Ok, "{\n\"chansons\":[\n"
        "{\n\"titre\":\"Never Gonna Give You Up\",\n\"artiste\":\"Foo\",\n\"duree\":\"4:20\",\n\"proposeePar\":\"Chuck Norris\",\n\"proprietaire\":false,\n\"no\":42},\n"
        "{\n\"titre\":\"Hey Jude\",\n\"artiste\":\"Beatles\",\n\"duree\":\"7:05\",\n\"proposeePar\":\"Claude\",\n\"proprietaire\":true,\n\"no\":25}\n"
    "]\n}\n");
    std::cout << "getFileList function called, param is " << param << std::endl;
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    if (!request.headers().has("X-Auth-Token")) {
        response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" missing");
    }
    else {
        uint32_t token = 0;
        try {
            token = std::stoi(request.headers().getRaw("X-Auth-Token").value());

            Database& db = *Database::instance();

            std::stringstream encoded(request.body());
            std::stringstream decoded;
            Base64::Decode(encoded, decoded);
            StringID3v2Tag tag(decoded.str());
            if (!tag.isEmpty()) {
                std::cout << "It's tag id3v2 !!!" << std::endl;
                std::cout << "Title: " << tag.title() << std::endl;
            }
        }
        catch (std::logic_error &e) {
            response.send(Http::Code::Bad_Request, "Header \"X-Auth-Token\" must be a 32 bits integer");
        }
    }
    std::cout << "postFile function called" << std::endl;
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    std::cout << "deleteFile function called" << std::endl;
}
