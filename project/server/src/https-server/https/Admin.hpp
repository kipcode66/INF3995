#ifndef HTTPS_ADMIN_HPP
#define HTTPS_ADMIN_HPP

#include "SecureRestApi.hpp"
#include <rapidjson/document.h>

class Admin {
public:
    Admin(const Rest::Request& req) {
        rapidjson::Document jsonDocument;
        jsonDocument.Parse(req.body().c_str());
        bool fieldsValid = (jsonDocument.IsObject()
                         && jsonDocument.HasMember("usager")
                         && jsonDocument.HasMember("mot_de_passe")
                         && jsonDocument["usager"]       != '\0'
                         && jsonDocument["mot_de_passe"] != '\0');
        if (!fieldsValid) {
            throw std::runtime_error("missing fields");
        }
        auto token = req.headers().getRaw("X-Auth-Token").value();
        if (token.empty() || std::stoul(token) == 0) {
            throw std::runtime_error("invalid token");
        }

        this->usager = jsonDocument["usager"].GetString();
        this->mot_de_passe = jsonDocument["mot_de_passe"].GetString();
        this->id = std::stoul(token);
    }

    std::string usager;
    std::string mot_de_passe;
    uint32_t    id;
};
#endif // !HTTPS_ADMIN_HPP

