#include <doctest/doctest.h>
#include <fmt/core.h>
#include "bika/Postgres.h"
#include "bika/Config.h"


TEST_SUITE("Postgres") {

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Prepared Statements") {
    bika::Postgres db{"host=localhost port=5432 password=pwd123 user=postgres dbname=postgres"};
    bika::Config config{"config.yml"};

    db.loadPreparedStatements(YAML::Node{config.cfg("postgres.queries")});

    // read
    // nlohmann::json j = db.executePrepared("GetUserById", 1);
    // fmt::print("result: {}", j.dump());

    // write
    // db.executePrepared("CreateUser", "and", 66, false);

    // get all
    nlohmann::json j = db.executePrepared("GetAllUsers");
    fmt::print("result: {}", j.dump());
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Basic Query") {
    bika::Postgres db{"host=localhost port=5432 password=pwd123 user=postgres dbname=postgres"};

    nlohmann::json res = db.execute("SELECT id,name,age,married FROM Demo");
    fmt::print("result: {}", res.dump());
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Manual Usage") {
    bika::Postgres db{"localhost", "5432", "postgres", "pwd123", "postgres"};

    // read one
    {
        auto c = db.connection();
        auto t = db.transaction(c);

        auto[id, name, age] = t.query1<int, std::string, int>("SELECT id,name,age FROM Demo WHERE id = 1");
    }

    // read multiple
    {
        auto c = db.connection();
        auto t = db.transaction(c);

        for (const auto[id, name, age] : t.query<int, std::string, int>("SELECT id,name,age FROM Demo ORDER BY id")) {
            // ...
        }
    }

    // write
    {
        auto c = db.connection();
        auto t = db.transaction(c);

        t.exec("INSERT INTO Demo(name,age) VALUES('sfl', 404)");
        t.commit();
    }
}

} // Postgres