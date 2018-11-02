#ifndef HTTP_RESTAPIUTILS_HPP
#define HTTP_RESTAPIUTILS_HPP

#include <string>
#include <cstring>

namespace elevation {
namespace restApiUtils{

std::string generateSalt(int length);
std::string generateMd5Hash(const std::string& macAdress);
uint32_t generateId(const std::string& str);

} // namespace restApiUtils
} // namespace elevation

#endif // HTTP_RESTAPIUTILS_HPP
