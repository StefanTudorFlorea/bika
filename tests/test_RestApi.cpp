#include <doctest/doctest.h>
#include "bika/RestApi.h"
#include <fmt/core.h>

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("RestApi") {
    bika::RestApi api;

    api.POST("/ping/{id}", [](auto req) -> bika::RestApi::Response {

        return {200, {
            {"statusCode", 200},
            {"message", "pong"},
            {"payload", req.body}
        }};
    });

    api.start("0.0.0.0", 8080);
}