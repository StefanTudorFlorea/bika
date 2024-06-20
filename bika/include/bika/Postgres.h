#pragma once
#include <string_view>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <pqxx/pqxx>
#include <mutex>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>


namespace bika {

/*  Connect to postgres database and allow to perform transactions
    Uses a connection from the pool automatically for a transaction

    Usage:
        Postgres db{...};
        auto t = db.transaction();
        t.query1, t.query, t.exec

    See: https://github.com/jtv/libpqxx
*/
class Postgres {
public:
    Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname);

    // optional: load a configuration with queryName: queryValue format
    void loadPreparedStatements(const YAML::Node& queries);

    // a connection can have only one transaction open at the same time
    pqxx::work transaction();

    // execute a prepared statement
    template<typename... Args>
    nlohmann::json executePrepared(const std::string& statement, Args &&...args) {
        pqxx::work t = transaction();
        pqxx::result r = t.exec_prepared(statement, args...);
        t.commit();

        return to_json(r);
    }
    nlohmann::json to_json(const pqxx::result& result);

private:
    std::string _connectionString;
    std::once_flag _initPool;
    std::unordered_map<std::string, std::string> _preparedStatements;

private:
    class ConnectionPool {
        public:
            static ConnectionPool& instance();
            void init(const std::string& connectionString, int poolSize);
            pqxx::connection& getConnection();

            void prepareStatements(const std::unordered_map<std::string, std::string>& statements);

        private:
            ConnectionPool() = default;
            ConnectionPool(const ConnectionPool&) = delete;

        private:
            std::vector<std::unique_ptr<pqxx::connection>> _pool;
            std::vector<std::unique_ptr<pqxx::connection>>::iterator _next;
            std::mutex _mtx;
    };
};

} // ns bika