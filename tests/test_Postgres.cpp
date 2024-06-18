#include <doctest/doctest.h>
#include "bika/Postgres.h"


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