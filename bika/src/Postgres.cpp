#include "bika/Postgres.h"
#include <fmt/core.h>
#include <thread>


namespace bika {

Postgres::Postgres(std::string_view host, std::string_view port, std::string_view user, std::string_view password, std::string_view dbname) {

    _connectionString = fmt::format("host={} port={} user={} password={} dbname={}", host, port, user, password, dbname);

    std::call_once(Postgres::_init, [c = _connectionString](){
        Postgres::initPool(c);
    });
}

void Postgres::initPool(const std::string& connectionString) {

    const int maxConnections = std::thread::hardware_concurrency();
    for (int i = 0; i < maxConnections; ++i) {
        _pool.push_back(std::make_unique<pqxx::connection>(connectionString));
    }

    _next = _pool.begin();
}

pqxx::connection& Postgres::connection() {
    std::lock_guard l{_mtx};

    if (Postgres::_next == _pool.end()) {
        Postgres::_next = _pool.begin();
    }

    return *(Postgres::_next++)->get();
}

pqxx::work Postgres::transaction() {
    return pqxx::work{Postgres::connection()};
}

} // ns bika