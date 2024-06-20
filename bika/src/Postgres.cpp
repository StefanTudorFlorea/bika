#include "bika/Postgres.h"
#include <fmt/core.h>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace bika {

Postgres::Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) {
    _connectionString = fmt::format("host={} port={} user={} password={} dbname={}", host, port, user, password, dbname);
}

pqxx::connection Postgres::connection() {
    pqxx::connection c = pqxx::connection{_connectionString};

    // each connection must be aware of the optional prepared statements
    for (const auto[name, query] : _preparedStatements) {
        c.prepare(name, query);
    }
    return c;
}

pqxx::work Postgres::transaction(pqxx::connection& conn) {
    return pqxx::work{conn};
}

void Postgres::loadPreparedStatements(const YAML::Node& queries) {

    // scan for the statements
    for (const auto& n : queries) {
        std::string name = n.first.as<std::string>();
        std::string query = n.second.as<std::string>();

        _preparedStatements.insert({name, query});
    }
}

nlohmann::json Postgres::to_json(const pqxx::row& row) {

    // Postgres internal OID types: https://github.com/olt/libpq/blob/master/oid/types.go
    enum DataType {
        T_bool = 16,
        T_int4 = 23,
        T_text = 25
    };

    // scan each column in the row for name and type so we can assign to json
    nlohmann::json jrow;
    for (const pqxx::field& field : row) {

        const std::string colName = field.name();
        const pqxx::oid colType   = field.type();

        // starts the value as string and later we see if we can find a more suitable type
        const std::string colVal  = field.as<std::string>();
        jrow[colName] = colVal;


        // if we can convert the type, let's create a better type
        if (colType == DataType::T_bool) jrow[colName] = field.as<bool>();
        if (colType == DataType::T_int4) jrow[colName] = field.as<int>();
        if (colType == DataType::T_text) jrow[colName] = field.as<std::string>();
    }

    // done
    return jrow;
}

nlohmann::json Postgres::to_json(const pqxx::result& result) {

    // no result
    if (result.empty()) {
        return {};
    }

    // return simple json, no need for array
    if (result.size() == 1) {
        return to_json(result.at(0));
    }

    // must return array as result
    nlohmann::json jarray;
    for (const pqxx::row& row : result) {
        jarray.push_back(to_json(row));
    }
    return jarray;
}

} // ns bika