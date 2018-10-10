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
    void createDescription_();
    void getIdentification_(const Rest::Request& request, Http::ResponseWriter response);
    void getFileList_(const Rest::Request& request, Http::ResponseWriter response);
    void postFile_(const Rest::Request& request, Http::ResponseWriter response);
    void deleteFile_(const Rest::Request& request, Http::ResponseWriter response);

    std::shared_ptr<Http::Endpoint> m_httpEndpoint;
    Rest::Description m_desc;
    Rest::Router m_router;
};


#endif // HTTP_RESTAPI_HPP
