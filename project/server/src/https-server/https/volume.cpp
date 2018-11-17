#include "SecureRestApi.hpp"

using namespace elevation;

void SecureRestApi::getSuperviseurVolume_(const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "getSuperviseurVolume_ called");
}

void SecureRestApi::postSuperviseurVolumeAug_ (const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Up");
}

void SecureRestApi::postSuperviseurVolumeDim_ (const Rest::Request& request, Http::ResponseWriter response) {
    response.send(Http::Code::Ok, "volume Down");
}

