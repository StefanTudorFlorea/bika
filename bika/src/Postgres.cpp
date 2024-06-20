#include "bika/Postgres.h"
#include <fmt/core.h>
#include <thread>


namespace bika {

Postgres::Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) {

    // initialize the pool with connections
    _connectionString = fmt::format("host={} port={} user={} password={} dbname={}", host, port, user, password, dbname);
    std::call_once(_initPool, [c = _connectionString](){
        Postgres::ConnectionPool::instance().init(c, 16);
    });
}

pqxx::work Postgres::transaction() {
    return pqxx::work{Postgres::ConnectionPool::instance().getConnection()};
}

void Postgres::loadPreparedStatements(const YAML::Node& queries) {

    // scan for the statements
    for (const auto& n : queries) {
        std::string name = n.first.as<std::string>();
        std::string query = n.second.as<std::string>();

        _preparedStatements.insert({name, query});
    }

    // add them to all connections
    Postgres::ConnectionPool::instance().prepareStatements(_preparedStatements);
}

nlohmann::json Postgres::to_json(const pqxx::result& result) {

    // helper: convert one row to json
    auto convertRowToJson = [](const pqxx::row& row) -> nlohmann::json {
        nlohmann::json jrow;

        // scan each column in the row for name and type so we can assign to json
        for (const pqxx::field& field : row) {

            // TODO: convert to correct type: https://github.com/olt/libpq/blob/master/oid/types.go
            const std::string colName = field.name();
            const pqxx::oid colType   = field.type();
            const std::string colVal  = field.as<std::string>();

            jrow[colName] = colVal;
        }

        // done
        return jrow;
    };

    // no result
    if (result.empty()) {
        return {};
    }

    // return simple json, no need for array
    if (result.size() == 1) {
        return convertRowToJson(result.at(0));
    }

    // must return array as result
    nlohmann::json jarray;
    for (const pqxx::row& row : result) {
        jarray.push_back(convertRowToJson(row));
    }
    return jarray;
}


//---------------------------------------------------------------------------------------------------------------------
// Connection Pool
//---------------------------------------------------------------------------------------------------------------------
Postgres::ConnectionPool &Postgres::ConnectionPool::instance() {
    static Postgres::ConnectionPool instance;
    return instance;
}

void Postgres::ConnectionPool::prepareStatements(const std::unordered_map<std::string, std::string>& statements) {
    for (std::unique_ptr<pqxx::connection>& c : _pool) {
        for (auto[statementName, statementQuery] : statements) {
            c.get()->prepare(statementName, statementQuery);
        }
    }
}

void Postgres::ConnectionPool::init(const std::string& connectionString, int poolSize) {
    for (int i = 0; i < poolSize; ++i) {
        _pool.push_back(std::make_unique<pqxx::connection>(connectionString));
    }
    _next = _pool.begin();
}

pqxx::connection& Postgres::ConnectionPool::getConnection() {
    std::lock_guard l{_mtx};

    if (_next == _pool.end()) {
        _next = _pool.begin();
    }

    return *(_next++)->get();
}

} // ns bika