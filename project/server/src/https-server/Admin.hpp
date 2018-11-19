#ifndef HTTPS_ADMIN_HPP
#define HTTPS_ADMIN_HPP

#include "https/SecureRestApi.hpp"
#include <rapidjson/document.h>

class Admin {
public:
    Admin(const Rest::Request&);

    std::string getUsername() const;
    std::string getMotDePasse() const;
    uint32_t    getId() const;

private:
    std::string m_username;
    std::string m_motDePasse;
    uint32_t    m_id;
};
#endif // !HTTPS_ADMIN_HPP

