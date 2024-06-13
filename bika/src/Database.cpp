#include "bika/Database.h"
#include <fmt/core.h>


namespace bika {

Database::Database(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) 
    : _params{host, port, user, password, dbname} {
    // empty
}

pqxx::connection Database::connection() {
    return pqxx::connection(
        fmt::format("host={} port={} user={} password={} dbname={}",
            _params.host, _params.port, _params.user, _params.password, _params.dbname));
}

pqxx::work Database::transaction(pqxx::connection& connection) {
    return pqxx::work{connection};
}

} // ns bika