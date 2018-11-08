#ifndef DATABASE_USER_HPP
#define DATABASE_USER_HPP

#include <stdint.h>
#include <cstddef>

struct User_t {
    static const size_t IP_LENGTH = 16;
    static const size_t MAC_LENGTH = 18;
    static const size_t NAME_LENGTH = 256;

    uint32_t userId;
    char ip[IP_LENGTH];
    char mac[MAC_LENGTH];
    char name[NAME_LENGTH];
};

#endif // !DATABASE_USER_HPP