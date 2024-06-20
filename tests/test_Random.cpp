#include <doctest/doctest.h>
#include <iostream>
#include "bika/Random.h"
#include <fmt/core.h>
#include <vector>

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Random") {
    std::vector<int> values{2,4,6,8};
    for (int i = 0; i < 100; ++i) {
        fmt::print("{}, ", bika::Random::choice(values));
    }
}
