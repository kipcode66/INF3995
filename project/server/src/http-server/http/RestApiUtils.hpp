#ifndef HTTP_RESTAPIUTILS_HPP
#define HTTP_RESTAPIUTILS_HPP

#include <string>
#include <cstring>

namespace elevation {
namespace restApiUtils{

std::string generateSalt(int length);
std::string generateToken(const std::string& macAdress);

} // namespace restApiUtils
} // namespace elevation

#endif // HTTP_RESTAPIUTILS_HPP
