#include "RestApiUtils.hpp"
#include <random>
#include <openssl/md5.h>
#include <functional>

namespace elevation {
namespace restApiUtils{

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

std::string generateMd5Hash(const std::string& macAdress) {
    uint32_t lengthOfMd5Hash = 32;
    std::string salt = generateSalt(macAdress.length());
    std::string macAdressTemp = macAdress + salt;
    char stringToHash[macAdress.length() + salt.length()];
    strcpy(stringToHash, macAdressTemp.c_str()); 
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)&stringToHash, strlen(stringToHash), (unsigned char*)&digest);    
    char token[lengthOfMd5Hash];
    for(int i = 0; i < 16; i++)
         sprintf(&token[i*2], "%02x", (unsigned int)digest[i]);
    return token;
}

uint32_t generateId(const std::string& macAdress) {
    std::hash<std::string> stringToIntegerHasher;
    return (uint32_t)stringToIntegerHasher(generateMd5Hash(macAdress));

}

}
}


