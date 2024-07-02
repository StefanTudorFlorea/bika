#include <doctest/doctest.h>
#include "bika/RestApi.h"
#include <fmt/core.h>

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("RestApi") {
    bika::RestApi api;

    api.setPreHandler([](const bika::RestApi::Request& req) -> bika::RestApi::Response {
        fmt::println("Auth: {}", std::string{req.headers["Authorization"]});
        return {400, {
            {"statusCode", 400},
            {"message", "Unauthorized"},
        }};
    });

    api.POST("/ping/{id}", [](auto req) -> bika::RestApi::Response {

        return {200, {
            {"statusCode", 200},
            {"message", "pong"},
            {"payload", req.body}
        }};
    });

    api.start("0.0.0.0", 8080);
}