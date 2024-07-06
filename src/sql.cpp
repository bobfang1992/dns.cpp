
#include "sql.h"
#include "sqlite3.h"
namespace dnscpp {

SQL::SQL() { sqlite3_initialize(); }

SQL::~SQL() { sqlite3_shutdown(); }

std::string SQL::version() { return sqlite3_libversion(); }

} // namespace dnscpp
