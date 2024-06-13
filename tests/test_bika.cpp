#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "bika/Config.h"


TEST_CASE("Config") {
    bika::Config config{"config.yml"};

    // value must be there
    int port = config.get<int>("configuration.ports.web");
    CHECK(port == 8080);
}