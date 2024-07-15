#include <doctest/doctest.h>
#include "bika/HttpServer.h"
#include <fmt/core.h>
#include <iostream>

TEST_CASE("json") {
    json ex {
        {"happy", true},
        {"pi", 3.141},
    }; 

    json ret = {
            {"status", 200},
            {"body", {
                {"message", "pong"},
                {"payload", "payload"}
            }},
            {"headers", {"H1:v1", "H2:v2", "H3:ve"}}
        };

    fmt::println("json: {}", ret.dump());
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("HttpServer") {
    bika::HttpServer api;

    // api.setPreHandler([](const bika::HttpServer::Request& req) -> json {
    //     fmt::println("Auth: {}", std::string{req.headers["Authorization"]});
    //     return {400, {
    //         {"statusCode", 400},
    //         {"message", "Unauthorized"},
    //     }};
    // });

    api.POST("/ping/{id}", [](auto req) -> json {
       json ret = {
            {"body", {
                {"message", "pong"},
                {"payload", req.body}
            }},
            {"headers", {"Cache-Control: remove-after: 2s",
                         "x-Max-age: 23s, 4min, 5h"
                        }
            }
        };

        std::cout << "json: " << ret.dump() << std::endl;

        return ret;
    });

    api.POST("/headers", [](auto req) -> json {
    
        return {200, 
            // body
            { 
                {"statusCode", 200},
                {"message", "pong"},
                {"payload", req.body}
            },
            // headers
            {
                {"Access-Control-Allow-Origin", "*"}
            }
        };
    });

    api.start("0.0.0.0", 8080);
}