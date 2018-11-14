#include <sstream>

#include "Statement.hpp"
#include "thread_safe_sqlite.hpp"
#include "sqlite_error.hpp"

using namespace elevation;

Statement::Statement(sqlite3* db, const Query& query) {
    const auto& q = query.value();
    m_currErrcode = sqlite3_blocking_prepare_v2(db, q.c_str(), q.length(), &m_stmt, NULL);
    assertSqliteOk_(m_currErrcode);
}
Statement::Statement(Statement&& that) {
    this->operator=(std::move(that));
}
Statement::~Statement() {
    m_currErrcode = sqlite3_finalize(m_stmt);
    // We don't call assertSqliteOk_ because we don't want the destructor to
    // throw an exception, for example, in the case the statement is detroyed
    // because of a previous error thrown by this instance.
}

Statement& Statement::operator=(Statement&& that) {
    m_currErrcode = sqlite3_finalize(this->m_stmt);
    this->m_stmt = that.m_stmt;
    that .m_stmt = nullptr;
    return *this;
}

bool Statement::getColumnBool(int col) const {
    return (bool)sqlite3_column_int(m_stmt, col);
}
int32_t Statement::getColumnInt(int col) const {
    return sqlite3_column_int(m_stmt, col);
}
int64_t Statement::getColumnInt64(int col) const {
    return sqlite3_column_int64(m_stmt, col);
}
std::string Statement::getColumnText(int col) const {
    return (const char*)sqlite3_column_text(m_stmt, col);
}
std::wstring Statement::getColumnText16(int col) const {
    return (const wchar_t*)sqlite3_column_text16(m_stmt, col);
}
double Statement::getColumnDouble(int col) const {
    return sqlite3_column_double(m_stmt, col);
}
int Statement::getColumnBytes(int col) const {
    return sqlite3_column_bytes(m_stmt, col);
}
int Statement::getColumnBytes16(int col) const {
    return sqlite3_column_bytes16(m_stmt, col);
}
const void* Statement::getColumnBlob(int col) const {
    return sqlite3_column_blob(m_stmt, col);
}

int Statement::getColumnType(int col) const {
    return sqlite3_column_type(m_stmt, col);
}
std::string Statement::getColumnName(int col) const {
    return sqlite3_column_name(m_stmt, col);
}

bool Statement::step() {
    m_currErrcode = sqlite3_blocking_step(m_stmt);
    assertSqliteOk_(m_currErrcode);
    return m_currErrcode == SQLITE_ROW;
}

void Statement::assertSqliteOk_(int errcode, const std::string& message) {
    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        std::ostringstream what;
        what << message << "; Sqlite error message: " << sqlite3_errstr(errcode);
        throw sqlite_error(errcode, what.str());
    }
}
void Statement::assertSqliteOk_(int errcode) {
    if (errcode != SQLITE_DONE
        && errcode != SQLITE_OK
        && errcode != SQLITE_ROW) {
        throw sqlite_error(errcode);
    }
}
