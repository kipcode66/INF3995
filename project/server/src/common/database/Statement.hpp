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

    bool getColumnBool(size_t) const;
    int32_t getColumnInt(size_t) const;
    int64_t getColumnInt64(size_t) const;
    std::string getColumnText(size_t) const;
    std::wstring getColumnText16(size_t) const;
    double getColumnDouble(size_t) const;
    int8_t getColumnBytes(size_t) const;
    int16_t getColumnBytes16(size_t) const;
    const void* getColumnBlob(size_t) const;

    int getColumnType(size_t) const; // sqlite3_column_type
    std::string getColumnName(size_t) const;

    bool step();

    void finalize();

protected:
    sqlite3_stmt* m_stmt = nullptr;
    int m_currErrcode = SQLITE_OK;

};

}

#endif // !COMMON_DATABASE_STATEMENT_HPP