#include "Query.hpp"

using namespace elevation;

Query::Query(const Query&) { }
Query::Query(Query&&) { }
Query::~Query() { }

Query& Query::operator=(const Query&) { }
Query& Query::operator=(Query&&) { }

std::string Query::value() const { }
