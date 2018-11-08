#ifndef DATABASE_SQLITE_ERROR_HPP
#define DATABASE_SQLITE_ERROR_HPP

#include <stdexcept>

namespace elevation {

class sqlite_error : public std::runtime_error {
public:
    sqlite_error();
    sqlite_error(int);
    sqlite_error(int, const std::string&);
    sqlite_error(sqlite_error&&) = default;
    sqlite_error(const sqlite_error&) = default;
    virtual ~sqlite_error();

    sqlite_error& operator=(const sqlite_error&) = default;
    sqlite_error& operator=(sqlite_error&&) = default;

protected:
    int m_code;
};

}

#endif // !DATABASE_SQLITE_ERROR_HPP