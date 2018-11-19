#include "https/SecureRestApi.hpp"
#include <rapidjson/document.h>

#include "Admin.hpp"

Admin::Admin(const Rest::Request& req) {
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

    this->m_username = jsonDocument["usager"].GetString();
    this->m_motDePasse = jsonDocument["mot_de_passe"].GetString();
    this->m_id = std::stoul(token);
}

std::string Admin::getUsername() const {
    return this->m_username;
}

std::string Admin::getMotDePasse() const {
    return this->m_motDePasse;
}

uint32_t Admin::getId() const {
    return this->m_id;
}

