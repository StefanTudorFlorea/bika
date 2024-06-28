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
        std::vector<int> val = config.cfg("types.listIntVar");
        CHECK(val == std::vector<int>{1,2,3,4});
    }

    SUBCASE("listStringVar") {
        std::vector<std::string> val = config.cfg("types.listStringVar");
        CHECK(val == std::vector<std::string>{"a", "b", "c"});
    }

}

TEST_CASE("Missing Key") {
    Config2 config;
    config.setFile("config.yml");


    CHECK_THROWS_AS(static_cast<int>(config.cfg("types.missingVar")), std::runtime_error);
    CHECK_THROWS_AS(static_cast<int>(config.env("MISSING_ENV")), std::runtime_error);
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

    SUBCASE("listIntVar") {
        std::vector<int> val = config.val(std::vector<int>{1,2,3,4});
        CHECK(val == std::vector<int>{1,2,3,4});
    }

    SUBCASE("listStringVar") {
        std::vector<std::string> val = config.val(std::vector<std::string>{"a", "b", "c"});
        CHECK(val == std::vector<std::string>{"a", "b", "c"});
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

TEST_CASE("Precedence") {
    Config2 config;
    config.setFile("config.yml");

    SUBCASE("cfg->val") {
        int val = config.cfg("types.intVar").val(42);
        CHECK(val == 42);
    }
    SUBCASE("val->cfg") {
        int val = config.val(42).cfg("types.intVar");
        CHECK(val == 101);
    }

    SUBCASE("cfg->env") {
        setenv("INT_VAR", "42", 1);
        int val = config.cfg("types.intVar").env("INT_VAR");
        CHECK(val == 42);
    }
    SUBCASE("env->cfg") {
        setenv("INT_VAR", "42", 1);
        int val = config.env("INT_VAR").cfg("types.intVar");
        CHECK(val == 101);
    }

    SUBCASE("env->val") {
        setenv("INT_VAR", "42", 1);
        int val = config.env("INT_VAR").val(101);
        CHECK(val == 101);
    }
    SUBCASE("val->env") {
        setenv("INT_VAR", "42", 1);
        int val = config.val(101).env("INT_VAR");
        CHECK(val == 42);
    }

    SUBCASE("chain") {
        setenv("INT_VAR", "42", 1);
        int val = config.val(1).cfg("types.intVar").val(3).cfg("INT_VAR").cfg("types.Missing").val(123);
        CHECK(val == 123);
    }
}

} // Config2