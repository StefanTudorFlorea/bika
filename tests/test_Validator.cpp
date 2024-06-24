#include <doctest/doctest.h>
#include <fmt/core.h>
#include "bika/Validator.h"
#include "bika/Config.h"

TEST_CASE("Validator - json") {
    bika::Validator v{"config.yml"};
    nlohmann::json person={{"name","stefan"}};
    v.validate(person, "validators.Schema");
}