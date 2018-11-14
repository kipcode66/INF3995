#ifndef COMMON_DATABASE_STATEMENT_HPP
#define COMMON_DATABASE_STATEMENT_HPP

#include <string>
#include <sqlite3/sqlite3.h>

#include "Query.hpp"

namespace elevation {

class Statement {
public:
    Statement(sqlite3* db, const Query& query);
    Statement(const Statement&) = delete;
    Statement(Statement&&);
    virtual ~Statement();

    Statement& operator=(const Statement&) = delete;
    Statement& operator=(Statement&&);

    bool getColumnBool(int) const;
    int32_t getColumnInt(int) const;
    int64_t getColumnInt64(int) const;
    std::string getColumnText(int) const;
    std::wstring getColumnText16(int) const;
    double getColumnDouble(int) const;
    int getColumnBytes(int) const;
    int getColumnBytes16(int) const;
    const void* getColumnBlob(int) const;

    int getColumnType(int) const; // sqlite3_column_type
    std::string getColumnName(int) const;

    bool step();

protected:
    sqlite3_stmt* m_stmt = nullptr;
    int m_currErrcode = SQLITE_OK;

    void assertSqliteOk_(int errcode, const std::string& message);
    void assertSqliteOk_(int errcode);
};

}

#endif // !COMMON_DATABASE_STATEMENT_HPP