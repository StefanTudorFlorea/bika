#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cstdlib>
#include "bika/Config.h"
#include "bika/RestApi.h"
#include "bika/Postgres.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Config") {
    bika::Config config{"config.yml"};

    // static value must be there - int
    CHECK(config.get<int>("configuration.ports.web") == 8080);

    // static not there, take default
    CHECK(config.get<int>("path.does.not.exist", {}, 42) == 42);

    // static not given, take default
    CHECK(config.get<int>({}, {}, 42) == 42);

    // static there, take it even if default is present
    CHECK(config.get<int>("configuration.ports.web", {}, 42) == 8080);

    // nothing found, and no defaults are set, throw exception
    CHECK_THROWS_AS(config.get<int>("path.does.not.exist"), std::exception);

    // env var overwrites static settings
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>("configuration.ports.web", "CUSTOM_PORT") == 1234);

    // env var overwrites static settings even if we set default
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>("configuration.ports.web", "CUSTOM_PORT", 42) == 1234);

    // env var takes precedence if static not found
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>("path.does.not.exist", "CUSTOM_PORT") == 1234);

    // env var takes precedence if static not found and even if we have default
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>("path.does.not.exist", "CUSTOM_PORT", 42) == 1234);

    // static not given, take env
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>({}, "CUSTOM_PORT", 42) == 1234);

    // static not given, default not given
    setenv("CUSTOM_PORT", "1234", 1);
    CHECK(config.get<int>({}, "CUSTOM_PORT", {}) == 1234);

    // check types
    CHECK(config.get<std::string>("types.stringVar") == "hello world");
    CHECK(config.get<int>("types.intVar") == 101);
    CHECK(config.get<bool>("types.boolVar") == true);
    CHECK(config.get<std::vector<int>>("types.listIntVar") == std::vector<int>{1,2,3,4});
    CHECK(config.get<std::vector<std::string>>("types.listStringVar") == std::vector<std::string>{"a", "b", "c"});
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("RestApi") {
    bika::RestApi api;

    api.add("GET", "/hello", [](auto req, auto& res) -> json {
        return {
            {"status", 200},
            {"message", "hello world"}
        };
    });

    api.add("GET", "/ping", [](auto req, auto& res) {
        return "OK";
    });

    // api.start("0.0.0.0", 8080);

    // test client
    auto[status, text] = bika::RestApi::call("GET", "http://www.httpbin.org/get", {}, cpr::Parameters{{"hello", "world"}});
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Database") {
    bika::Postgres db{"localhost", "5432", "admin", "pwd123", "data"};

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