#include <rapidjson/document.h>

#include "Admin.hpp"

#include <common/database/Database.hpp>

#include <http-server/http/exception/MissingTokenException.hpp>
#include <http-server/http/exception/InvalidTokenException.hpp>

#include "https/exception/AuthenticationFailureException.hpp"

namespace elevation {

Admin Admin::extractAdminDataFromRequest(const Pistache::Rest::Request& request) {
    rapidjson::Document jsonDocument;
    jsonDocument.Parse(request.body().c_str());
    bool fieldsValid = (jsonDocument.IsObject()
                     && jsonDocument.HasMember("usager")
                     && jsonDocument.HasMember("mot_de_passe")
                     && jsonDocument["usager"]       != '\0'
                     && jsonDocument["mot_de_passe"] != '\0');
    if (!fieldsValid) {
        throw std::runtime_error("missing fields");
    }
    auto token = request.headers().getRaw("X-Auth-Token").value();
    if (token.empty() || std::stoul(token) == 0) {
        throw std::runtime_error("invalid token");
    }

    std::string username   = jsonDocument["usager"].GetString();
    std::string motDePasse = jsonDocument["mot_de_passe"].GetString();
    uint32_t id = std::stoul(token);

    return Admin{username, motDePasse, id};
}

Admin Admin::getAdminDataFromRequestToken(const Pistache::Rest::Request& request) {
    auto token = request.headers().getRaw("X-Auth-Token").value();
    if (token.empty()) {
        throw MissingTokenException{};
    }
    uint32_t id = std::stoul(token);
    if (id == 0) {
        throw InvalidTokenException{token};
    }

    Database* db = Database::instance();
    if (!db->isAdminConnected(id)) {
        throw AuthenticationFailureException{token};
    }

    return Admin{"", "", id};
}

Admin::Admin(std::string username, std::string motDePasse, uint32_t id) {
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

} // namespace elevation
