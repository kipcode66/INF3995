//
// Created by adam on 04/10/18.
//

#ifndef HTTP_RESTAPI_HPP
#define HTTP_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>

using namespace Pistache;

class RestApi {
public:
    RestApi(Address);
    ~RestApi();
    void init();
    void start();

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


#endif // HTTP_RESTAPI_HPP
