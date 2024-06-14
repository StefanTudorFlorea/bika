#pragma once
#include <string_view>
#include <string>
#include <pqxx/pqxx>


namespace bika {

/*  Connect to postgres database and allow to perform transactions
    Create a connection per transaction

    Usage:
        Postgres db{...};
        auto c = db.connection();
        auto t = db.transaction(c);
        t.query1, t.query, t.exec

    See: https://github.com/jtv/libpqxx
*/
class Postgres {
public:
    struct ConnectionParams {
        std::string_view host;
        std::string_view port;
        std::string_view user;
        std::string_view password;
        std::string_view dbname;
    };
    Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname);

    // Input of form: "host={} port={} user={} password={} dbname={}" in any order, but with ws
    Postgres(std::string_view connectionString);

    // create a connection. we can have multiple open at the same time
    pqxx::connection connection() const;

    // a connection can have only one transaction open at the same time
    pqxx::work transaction(pqxx::connection& connection) const;

private:
    ConnectionParams _params;
    std::string _connectionString;
};

} // ns bika