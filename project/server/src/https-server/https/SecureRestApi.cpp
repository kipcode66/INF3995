#include "SecureRestApi.hpp"
#include "database/Database.hpp"

using namespace elevation;

SecureRestApi::SecureRestApi(Address addr)
: RestApi(addr)
{ }

//void RestApi::getFileList_(const Rest::Request& request, Http::ResponseWriter response) {
//    // querying a param from the request object, by name
//    std::string param = request.param(":id").as<std::string>();
//    response.send(Http::Code::Ok, "getFileList, param is : " + param);
//    std::cout << "getFileList function called, param is " << param << std::endl;
//}

