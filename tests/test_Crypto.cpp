#include <doctest/doctest.h>
#include <iostream>
#include "bika/Crypto.h"


//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Crypto") {
    std::string encoded = bika::Crypto::toBase64("hello world, this is stefan");
    CHECK(encoded == "aGVsbG8gd29ybGQsIHRoaXMgaXMgc3RlZmFu");

    std::string decoded = bika::Crypto::fromBase64("aGVsbG8gd29ybGQsIHRoaXMgaXMgc3RlZmFu");
    CHECK(decoded == "hello world, this is stefan");
}
