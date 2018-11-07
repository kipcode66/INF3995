#ifndef HTTP_IDUTILS_HPP
#define HTTP_IDUTILS_HPP

#include <string>
#include <cstring>

namespace elevation {
namespace id_utils{

std::string generateSalt(int length);
std::string generateMd5Hash(const std::string& str, const std::string& salt);
uint32_t generateId(const std::string& macAdress, const std::string& salt);

} // namespace id_utils
} // namespace elevation

#endif // HTTP_IDUTILS_HPP
