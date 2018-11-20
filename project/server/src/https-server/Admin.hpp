#ifndef HTTPS_ADMIN_HPP
#define HTTPS_ADMIN_HPP

#include <pistache/description.h>
#include <rapidjson/document.h>

namespace elevation {
class Admin {
public:
    static Admin extractAdminDataFromRequest (const Pistache::Rest::Request& request);
    static Admin getAdminDataFromRequestToken(const Pistache::Rest::Request& request);

public:
    std::string getUsername() const;
    std::string getMotDePasse() const;
    uint32_t    getId() const;

private:
    Admin(std::string username, std::string motDePasse, uint32_t id);

private:
    std::string m_username;
    std::string m_motDePasse;
    uint32_t    m_id;
};

} // namespace elevation

#endif // !HTTPS_ADMIN_HPP

