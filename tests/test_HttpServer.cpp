#include <doctest/doctest.h>
#include "bika/HttpServer.h"
#include <fmt/core.h>

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("HttpServer") {
    bika::HttpServer api;

    api.setPreHandler([](const bika::HttpServer::Request& req) -> bika::HttpServer::Response {
        fmt::println("Auth: {}", std::string{req.headers["Authorization"]});
        return {400, {
            {"statusCode", 400},
            {"message", "Unauthorized"},
        }};
    });

    api.POST("/ping/{id}", [](auto req) -> bika::HttpServer::Response {

        return {200, {
            {"statusCode", 200},
            {"message", "pong"},
            {"payload", req.body}
        }};
    });

    api.start("0.0.0.0", 8080);
}