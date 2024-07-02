#include <doctest/doctest.h>
#include "bika/RestApi.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("RestApi") {
    bika::RestApi api;

    api.POST("/ping", [](auto req) -> bika::RestApi::Response {
        return {200, {
            {"statusCode", 200},
            {"message", "pong"}
        }};
    });

    api.start("0.0.0.0", 8080);
}