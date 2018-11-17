#include "id_utils.hpp"
#include <random>
#include <openssl/md5.h>
#include <functional>

namespace elevation {
namespace id_utils {

std::string generateSalt(int length) {
    static auto& characters = 
        "0123456789"
        "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    thread_local static std::default_random_engine rg{std::random_device{}()};
    thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(characters) - 2);
    
    std::string salt;
    salt.reserve(length);
    for (int i = 0; i < length; i++)
        salt += characters[pick(rg)];

    return salt;
}

std::string generateMd5Hash(const std::string& str, const std::string& salt) {
    uint32_t lengthOfMd5Hash = 32;
    std::string strTemp = str + salt;
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)strTemp.c_str(), strTemp.size(), (unsigned char*)&digest);    
    char md5Hash[lengthOfMd5Hash];
    for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
         sprintf(&md5Hash[i*2], "%02x", (unsigned int)digest[i]);
    return md5Hash;
}

uint32_t generateId(const std::string& macAdress, const std::string& salt) {
    std::hash<std::string> stringToIntegerHasher;
    return (uint32_t)stringToIntegerHasher(generateMd5Hash(macAdress, salt));
}

}
}


