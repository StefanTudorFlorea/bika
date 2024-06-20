#include "bika/Postgres.h"
#include <fmt/core.h>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


//---------------------------------------------------------------------------------------------------------------------
// Connection Pool: used internally
//---------------------------------------------------------------------------------------------------------------------
class ConnectionPool {
public:
    static ConnectionPool& instance() {
        static ConnectionPool instance;
        return instance;
    }

    // initialize the pool with poolSize number of connections using connectionString = 'host={} port={} user={} password={} dbname={}'
    void init(const std::string& connectionString, int poolSize) {
        for (int i = 0; i < poolSize; ++i) {
            _pool.push_back(std::make_unique<pqxx::connection>(connectionString));
        }
        _next = _pool.begin();
    }

    // get next available connection from the pool
    pqxx::connection& getConnection() {
        std::lock_guard l{_mtx};
        if (_next == _pool.end()) {
            _next = _pool.begin();
        }
        return *(_next++)->get();
    }

    // load the prepared statements to each connection in the pool
    void prepareStatements(const std::unordered_map<std::string, std::string>& statements) {
        for (std::unique_ptr<pqxx::connection>& c : _pool) {
            for (auto[statementName, statementQuery] : statements) {
                c.get()->prepare(statementName, statementQuery);
            }
        }
    }

private:
    ConnectionPool() = default;
    ConnectionPool(const ConnectionPool&) = delete;

private:
    std::vector<std::unique_ptr<pqxx::connection>> _pool;
    std::vector<std::unique_ptr<pqxx::connection>>::iterator _next;
    std::mutex _mtx;
};


//---------------------------------------------------------------------------------------------------------------------
// Postgres
//---------------------------------------------------------------------------------------------------------------------
namespace bika {

Postgres::Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) {

    // initialize the pool with connections
    _connectionString = fmt::format("host={} port={} user={} password={} dbname={}", host, port, user, password, dbname);
    std::call_once(_initPool, [c = _connectionString](){
        ConnectionPool::instance().init(c, 16);
    });
}

pqxx::work Postgres::transaction() {
    return pqxx::work{ConnectionPool::instance().getConnection()};
}

void Postgres::loadPreparedStatements(const YAML::Node& queries) {

    // scan for the statements
    for (const auto& n : queries) {
        std::string name = n.first.as<std::string>();
        std::string query = n.second.as<std::string>();

        _preparedStatements.insert({name, query});
    }

    // add them to all connections
    ConnectionPool::instance().prepareStatements(_preparedStatements);
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