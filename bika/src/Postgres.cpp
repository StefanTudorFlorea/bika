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

Postgres::ConnectionPool& Postgres::ConnectionPool::instance() {
    static Postgres::ConnectionPool instance;
    return instance;
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