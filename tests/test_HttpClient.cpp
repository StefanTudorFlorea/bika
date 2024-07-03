#include <doctest/doctest.h>
#include <iostream>
#include "bika/HttpClient.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Http") {
    json params = {
        {"hello", "world"},
        {"name", "stefan"},
    };
    json body = {
        {"age",42},
        {"val","cool story"}
    };

    json headers = {
        {"Authorization", "Bearer your_token"},
        {"x-Custom", "my-custom"}, 
    };

    auto[status, text] = bika::HttpClient::GET("http://www.httaasaspbin.org/get", params, {}, body.dump());
    std::cout << "result2:" << status << std::endl;
}
