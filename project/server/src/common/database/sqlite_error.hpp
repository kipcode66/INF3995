#ifndef DATABASE_SQLITE_ERROR_HPP
#define DATABASE_SQLITE_ERROR_HPP

#include <stdexcept>

namespace elevation {

class sqlite_error : public std::runtime_error {
public:
    sqlite_error();
    sqlite_error(int);
    sqlite_error(int, const std::string&);
    virtual ~sqlite_error();

    int code() const;

protected:
    int m_code;
};

}

#endif // !DATABASE_SQLITE_ERROR_HPP