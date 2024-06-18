#pragma once
#include <string_view>
#include <string>
#include <memory>
#include <vector>
#include <pqxx/pqxx>
#include <mutex>

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

    // a connection can have only one transaction open at the same time
    pqxx::work transaction();

private:
    // get next available connection in the pool
    static pqxx::connection& connection();

    // create pool of connection
    static void initPool(const std::string& connectionString);

private:
    std::string _connectionString;

    // pool settings
    static inline std::vector<std::unique_ptr<pqxx::connection>> _pool;
    static inline std::vector<std::unique_ptr<pqxx::connection>>::iterator _next;
    static inline std::mutex _mtx;
    static inline std::once_flag _init;
};

} // ns bika