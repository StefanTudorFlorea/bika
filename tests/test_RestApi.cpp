#include <doctest/doctest.h>
#include "bika/RestApi.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("RestApi") {
    bika::RestApi api;

    api.POST("/ping", [](auto req) -> bika::RestApi::Response {
        return {200, "OK"};
    });

    // api.start("0.0.0.0", 8080);
}