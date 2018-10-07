//
// Created by adam on 04/10/18.
//

#ifndef SERVER_RESTAPI_H
#define SERVER_RESTAPI_H

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

using namespace Pistache;

class RestApi {
public:
    RestApi(Address);

    void init();

    void start();

    void shutdown();

private:

    void createDescription();
    void getIdentification(const Rest::Request& request, Http::ResponseWriter response);
    void getFileList(const Rest::Request& request, Http::ResponseWriter response);
    void postFile(const Rest::Request& request, Http::ResponseWriter response);
    void deleteFile(const Rest::Request& request, Http::ResponseWriter response);

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Description desc;
    Rest::Router router;
};


#endif //SERVER_RESTAPI_H
