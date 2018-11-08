#ifndef DATABASE_SQLITE_ERROR_HPP
#define DATABASE_SQLITE_ERROR_HPP

#include <stdexcept>

namespace elevation {

class sqlite_error : public std::runtime_error {
public:
    explicit sqlite_error(sqlite_error&&) = default;
    sqlite_error();
    sqlite_error(int);
    sqlite_error(int, const std::string&);
    virtual ~sqlite_error();

protected:
    int m_code;
};

}

#endif // !DATABASE_SQLITE_ERROR_HPP