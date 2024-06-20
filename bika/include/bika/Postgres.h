#pragma once
#include <mutex>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <string_view>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>


namespace bika {

/*  Connect to postgres database and allow to perform either manual or prepared transactions
    Can dynamically load prepared statements and uses a connection pool

    Usage-2: Loading prepared statements from a config file (preferred way)

        // GetUserById: SELECT id,name,age,married FROM Demo WHERE id = $1
        // CreateUser: INSERT INTO Demo(name,age,married) VALUES($1, $2, $3)
        bika::Config config{"config.yml"};
        db.loadPreparedStatements(config.get<YAML::Node>("postgres.queries"));

        // read
        nlohmann::json res = db.executePrepared("GetAllUsers");

        // write
        nlohmann::json res = db.executePrepared("CreateUser", ...);


    Usage-2: Manually creating statements using the library syntax
        Postgres db{...};
        auto t = db.transaction();
        t.query1, t.query, t.exec

    See: https://github.com/jtv/libpqxx
*/
class Postgres {
public:
    Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname);

    // usage-1: load a configuration with 'queryName: queryValue' format(optional)
    void loadPreparedStatements(const YAML::Node& queries);

    // execute a prepared statement from the pre-loaded prepared statements
    template<typename... Args>
    nlohmann::json executePrepared(const std::string& statement, Args &&...args) {
        pqxx::work t = transaction();
        
        // TODO: add support for transactions
        pqxx::result r = t.exec_prepared(statement, args...);
        t.commit();

        return to_json(r);
    }

    // usage-2: create manual transactions. Connection is automatically associated with the transaction
    pqxx::work transaction();

private:
    // convert result to json which makes it easier for the user to work with
    // @return json object or array depending if result returns one or multiple results
    nlohmann::json to_json(const pqxx::result& result);

private:
    std::string _connectionString; // format: 'host={} port={} user={} password={} dbname={}'
    std::once_flag _initPool; // initialize once the connection pool
    std::unordered_map<std::string, std::string> _preparedStatements; // Name -> SQL Query    
};

} // ns bika