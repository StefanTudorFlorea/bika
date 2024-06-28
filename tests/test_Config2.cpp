#include <doctest/doctest.h>
#include "bika/Config2.h"

TEST_SUITE("Config2") {


TEST_CASE("Cfg") {
    Config2 config;
    config.setFile("config.yml");

    SUBCASE("stringType") {
        std::string val = config.cfg("types.stringVar");
        CHECK(val == "hello world");
    }

    SUBCASE("intType") {
        int val = config.cfg("types.intVar");
        CHECK(val == 101);
    }

    SUBCASE("boolType") {
        bool val = config.cfg("types.boolVar");
        CHECK(val == true);
    }

    SUBCASE("listIntVar") {
        // std::vector<int> val = config.cfg("types.listIntVar");
        // CHECK(val == std::vector<int>{1,2,3,4});
    }
}

TEST_CASE("Val") {
    Config2 config;
    config.setFile("config.yml");

    SUBCASE("stringType") {
        std::string val = config.val("hello world");
        CHECK(val == "hello world");
    }

    SUBCASE("intType") {
        int val = config.val(101);
        CHECK(val == 101);
    }

    SUBCASE("boolType") {
        bool val = config.val(true);
        CHECK(val == true);
    }
}

TEST_CASE("Env") {
    Config2 config;
    config.setFile("config.yml");

    SUBCASE("stringType") {
        setenv("STRING_VAR", "hello world", 1);
        std::string val = config.env("STRING_VAR");
        CHECK(val == "hello world");
    }

    SUBCASE("intType") {
        setenv("INT_VAR", "101", 1);
        int val = config.env("INT_VAR");
        CHECK(val == 101);
    }

    SUBCASE("boolType") {
        setenv("BOOL_VAR", "true", 1);
        bool val = config.env("BOOL_VAR");
        CHECK(val == true);

        setenv("BOOL_VAR", "false", 1);
        val = config.env("BOOL_VAR");
        CHECK(val == false);
    }
}

} // Config2