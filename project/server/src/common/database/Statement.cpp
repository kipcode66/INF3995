#include <sstream>

#include "Statement.hpp"
#include "Database.hpp"
#include "thread_safe_sqlite.hpp"
#include "sqlite_error.hpp"

using namespace elevation;

Statement::Statement(sqlite3* db, const Query& query) {
    const auto& q = query.value();
    m_currErrcode = sqlite3_blocking_prepare_v2(db, q.c_str(), q.length(), &m_stmt, NULL);
    Database::assertSqliteOk(m_currErrcode);
}
Statement::Statement(Statement&& that) {
    this->operator=(std::move(that));
}
Statement::~Statement() {
    finalize();
    // We don't call assertSqliteOk because we don't want the destructor to
    // throw an exception, for example, in the case the statement is detroyed
    // because of a previous error thrown by this instance.
}

Statement& Statement::operator=(Statement&& that) {
    finalize();
    this->m_stmt = that.m_stmt;
    that .m_stmt = nullptr;
    return *this;
}

bool Statement::getColumnBool(size_t col) const {
    return (bool)sqlite3_column_int(m_stmt, col);
}
int32_t Statement::getColumnInt(size_t col) const {
    return sqlite3_column_int(m_stmt, col);
}
int64_t Statement::getColumnInt64(size_t col) const {
    return sqlite3_column_int64(m_stmt, col);
}
std::string Statement::getColumnText(size_t col) const {
    return (const char*)sqlite3_column_text(m_stmt, col);
}
std::wstring Statement::getColumnText16(size_t col) const {
    return (const wchar_t*)sqlite3_column_text16(m_stmt, col);
}
double Statement::getColumnDouble(size_t col) const {
    return sqlite3_column_double(m_stmt, col);
}
int8_t Statement::getColumnBytes(size_t col) const {
    return sqlite3_column_bytes(m_stmt, col);
}
int16_t Statement::getColumnBytes16(size_t col) const {
    return sqlite3_column_bytes16(m_stmt, col);
}
const void* Statement::getColumnBlob(size_t col) const {
    return sqlite3_column_blob(m_stmt, col);
}

int Statement::getColumnType(size_t col) const {
    return sqlite3_column_type(m_stmt, col);
}
std::string Statement::getColumnName(size_t col) const {
    return sqlite3_column_name(m_stmt, col);
}

bool Statement::step() {
    m_currErrcode = sqlite3_blocking_step(m_stmt);
    Database::assertSqliteOk(m_currErrcode);
    return m_currErrcode == SQLITE_ROW;
}

void Statement::finalize() {
    m_currErrcode = sqlite3_finalize(m_stmt);
    m_stmt = nullptr;
}
