#include <doctest/doctest.h>
#include <fmt/core.h>
#include "bika/Postgres.h"
#include "bika/Config.h"

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Postgres - dynamic query") {
    bika::Postgres db{"localhost", "5432", "admin", "pwd123", "users"};
    bika::Config config{"config.yml"};

    db.loadPreparedStatements(config.get<YAML::Node>("postgres.queries"));

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
TEST_CASE("Postgres") {
    bika::Postgres db{"localhost", "5432", "admin", "pwd123", "users"};

    // read one
    {
        auto t = db.transaction();

        auto[id, name, age] = t.query1<int, std::string, int>("SELECT id,name,age FROM Demo WHERE id = 1");
    }

    // read multiple
    {
        auto t = db.transaction();

        for (const auto[id, name, age] : t.query<int, std::string, int>("SELECT id,name,age FROM Demo ORDER BY id")) {
            // ...
        }
    }

    // write
    {
        auto t = db.transaction();

        t.exec("INSERT INTO Demo(name,age) VALUES('sfl', 404)");
        t.commit();
    }
}