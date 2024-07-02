#include <doctest/doctest.h>
#include <iostream>
#include "bika/Http.h"


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

    auto[status, text] = bika::Http::GET("http://www.httpbin.org/get", params, headers, body.dump());
    std::cout << "result2:" << text << std::endl;
}
