#include "Statement.hpp"
#include "thread_safe_sqlite.hpp"

using namespace elevation;

Statement::Statement(sqlite3* db, const Query& query) {
    int errcode = SQLITE_OK;
    const auto& q = query.value();
    errcode = sqlite3_blocking_prepare_v2(db, q.c_str(), q.length(), &m_stmt, NULL);
    assertSqliteOk_(errcode);
}
Statement::Statement(const Statement&) { }
Statement::Statement(Statement&&) { }
Statement::~Statement() { }

Statement& Statement::operator=(const Statement&) { }
Statement& Statement::operator=(Statement&&) { }

int32_t Statement::getColumnInt(int) const { }
int64_t Statement::getColumnInt64(int) const { }
std::string Statement::getColumnText(int) const { }
std::wstring Statement::getColumnText16(int) const { }
double Statement::getColumnDouble(int) const { }
int Statement::getColumnBytes(int) const { }
int Statement::getColumnBytes16(int) const { }
void* Statement::getColumnBlob(int) const { }

int Statement::getColumnType(int) const { }
std::string Statement::getColumnName(int) const { }

bool Statement::step() { }

void Statement::assertSqliteOk_(int errcode, const std::string& message) { }
void Statement::assertSqliteOk_(int errcode) { }
