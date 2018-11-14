#include "Query.hpp"
#include <cstring>

using namespace elevation;

Query::Query(const Query& that) {
    this->operator=(that);
}
Query::Query(Query&& that) {
    this->operator=(std::move(that));
}
Query::~Query() {
    sqlite3_free(m_query);
}

Query& Query::operator=(const Query& that) {
    auto n = sqlite3_msize(that.m_query);
    sqlite3_free(this->m_query);
    this->m_query = (char*)sqlite3_malloc(n);
    strncpy(this->m_query, that.m_query, n);
    return *this;
}
Query& Query::operator=(Query&& that) {
    sqlite3_free(this->m_query);
    this->m_query = that.m_query;
    that .m_query = nullptr;
    return *this;
}

std::string Query::value() const {
    return std::string(m_query);
}
