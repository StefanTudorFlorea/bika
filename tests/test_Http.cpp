#include <doctest/doctest.h>
#include <iostream>
#include "bika/Http.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Http") {
    auto[status, text] = bika::Http::GET("http://www.httpbin.org/get", json{{"hello", "world"}}, {}, "THIS IS MY BODY");
    std::cout << "result2:" << text << std::endl;
}
