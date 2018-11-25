#ifndef DATABASE_SQLITE_ERROR_HPP
#define DATABASE_SQLITE_ERROR_HPP

#include <stdexcept>

#include <sqlite3/sqlite3.h>

namespace elevation {

class sqlite_error : public std::runtime_error {
public:
    sqlite_error(int code = SQLITE_OK);
    sqlite_error(int, const std::string&);
    virtual ~sqlite_error();

    int code() const;

protected:
    int m_code;
};

}

#endif // !DATABASE_SQLITE_ERROR_HPP