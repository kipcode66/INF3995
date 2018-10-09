//
// Created by adam on 04/10/18.
//

#include <sqlite3/sqlite3.h>
#include "RestApi.hpp"

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

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

struct User {
    char* ip;
    char* mac;
    char* name;
};

void RestApi::getIdentification_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getIdentification");
    puts("getIdentification function called");

    struct User mocked_user = {
            .ip = "23.2.2.12",
            .mac = "FE:FE:FE:FE:FE:FE",
            .nom = "Jerry",
    };

    // Open db
    sqlite3* db;
    int rc = ::sqlite3_open("server.db", &db);
    if ( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    else {
        puts("database opened with ease\n");
    }


    // search for user in db

    const char* SQL_ADD_USER = "SELECT * FROM user WHERE (ip";
    char* sqlErrMsg = 0;
    rc = sqlite3_exec(db, SQL_ADD_USER, callback, NULL, &sqlErrMsg);
    if (rc != SQLITE_OK) {
        puts("that didn't go well");
        printf("SQL error: %s\n", sqlErrMsg);
        sqlite3_free(sqlErrMsg);
    }
    // create entry in user
    const char* SQL_ADD_USER = "INSERT INTO user (ip, mac, name) VALUES ('122.22.22.22', 'EE:FE:EE:EE:EE:EE', 'moi');";
    char* sqlErrMsg = 0;
    rc = sqlite3_exec(db, SQL_ADD_USER, callback, NULL, &sqlErrMsg);
    if (rc != SQLITE_OK) {
        puts("that didn't go well");
        printf("SQL error: %s\n", sqlErrMsg);
        sqlite3_free(sqlErrMsg);
    }
}

void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
    // querying a param from the request object, by name
    std::string param = request.param(":id").as<std::string>();
    response.send(Http::Code::Ok, "getFileList, param is : " + param);
    printf("getFileList function called, param is %s\n", param.c_str());
}

void RestApi::postFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "postFile");
    puts("postFile function called");
}

void RestApi::deleteFile_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "deleteFile");
    puts("deleteFile function called");
}
