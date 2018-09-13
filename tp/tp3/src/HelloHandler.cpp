#include "HelloHandler.hpp"

#include <regex>

#include "pistache/endpoint.h"

void HelloHandler::onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter response) {
    std::cout << "ressource \"" << request.resource() << '"' << std::endl;

    if (request.method() == Pistache::Http::Method::Get) {
        if (std::regex_match(request.resource(), std::regex("/test1/?"))) {
            response.send(Pistache::Http::Code::Ok, "test 1 est bon...\n");
        }
        else if (std::regex_match(request.resource(), std::regex("/test2/?"))) {
            response.send(Pistache::Http::Code::Ok, "<a href=\"http://www.polymtl.ca\">Poly Mtl</a>\n");
        }
        else if (std::regex_match(request.resource(), std::regex("/test3/?"))) {
            response.send(
                Pistache::Http::Code::Ok,
                "<!DOCTYPE html>\n"
                "<html>\n"
                "    <body>\n"
                "        <img "
                "          src=\"https://i.kym-cdn.com/photos/images/original/000/863/220/fbb.jpg\" "
                "          alt=\"This should have been a good joke about Chuck Norris, but Chuck Norris didn't want us to joke about him.\""
                "        />\n"
                "    </body>\n"
                "</html>\n"
            );
        }
        else {
            send404(response);
        }
    }
    else {
        send404(response);
    }
}

void HelloHandler::send404(Pistache::Http::ResponseWriter& response) const {
    response.send(
        Pistache::Http::Code::Not_Found,
        "<!DOCTYPE html>\n"
        "<html>\n"
        "    <body>\n"
        "        <div><p>404. Sorry, that's an error.<br/><h2>Sucks to be you</h2></p></div>\n"
        "        <img src=\"https://cdn-images-1.medium.com/max/1600/0*AnVCpSvrAeldg3Rn.\"/>\n"
        "    </body>\n"
        "</html>\n"
    );
}
