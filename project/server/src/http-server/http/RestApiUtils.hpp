#ifndef HTTP_RESTAPIUTILS_HPP
#define HTTP_RESTAPIUTILS_HPP

#include <string>
#include <cstring>

namespace elevation {
namespace restApiUtils{

std::string generateSalt(int length);
std::string hashMacAdress(const std::string& macAdress);

} // namespace restApiUtils
} // namespace elevation

#endif // HTTP_RESTAPIUTILS_HPP
