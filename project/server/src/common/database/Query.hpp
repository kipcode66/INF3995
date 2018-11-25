#ifndef COMMON_DATABASE_QUERY_HPP
#define COMMON_DATABASE_QUERY_HPP

#include <string>
#include <sqlite3/sqlite3.h>

namespace elevation {

class Query {
public:
    template<typename... Args> Query(const char* zFormat, Args... args) {
        m_query = sqlite3_mprintf(zFormat, args...);
    }
    Query(const Query&);
    Query(Query&&);
    virtual ~Query();

    Query& operator=(const Query&);
    Query& operator=(Query&&);

    std::string value() const;

protected:
    char* m_query = nullptr;
};

}

#endif // !COMMON_DATABASE_QUERY_HPP
