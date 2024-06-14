#include "bika/Postgres.h"
#include <fmt/core.h>


namespace bika {

Postgres::Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) 
    : _params{host, port, user, password, dbname} {

    _connectionString = fmt::format("host={} port={} user={} password={} dbname={}",
                                    _params.host, _params.port, _params.user, _params.password, _params.dbname);
}

Postgres::Postgres(std::string_view connectionString) 
    : _connectionString{connectionString} {
        
    // TODO: parse string to ConnectionParams
}

pqxx::connection Postgres::connection() const {
    return pqxx::connection(_connectionString);
}

pqxx::work Postgres::transaction(pqxx::connection& connection) const {
    return pqxx::work{connection};
}

} // ns bika