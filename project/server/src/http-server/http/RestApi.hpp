#ifndef HTTP_RESTAPI_HPP
#define HTTP_RESTAPI_HPP

#include <pistache/net.h>
#include <pistache/endpoint.h>
#include <pistache/router.h>
#include <pistache/description.h>
#include <taglib/id3v2tag.h>

using namespace Pistache;
namespace elevation {

class StringID3v2Tag : public TagLib::ID3v2::Tag {
public:
    StringID3v2Tag(const std::string& data);
};

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

} // namespace elevation

#endif // HTTP_RESTAPI_HPP
