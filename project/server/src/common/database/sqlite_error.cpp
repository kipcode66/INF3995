#include "sqlite_error.hpp"

namespace elevation {

sqlite_error::sqlite_error(int code)
    : runtime_error(sqlite3_errstr(code))
    , m_code(code)
{ }

sqlite_error::sqlite_error(int code, const std::string& msg)
    : runtime_error(msg)
    , m_code(code)
{ }

sqlite_error::~sqlite_error()
{ }

int sqlite_error::code() const {
    return m_code;
}

}