#include <doctest/doctest.h>
#include <iostream>
#include "bika/Crypto.h"

TEST_SUITE("Crypto") {

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Base64 encode/decode") {
    std::string encoded = bika::Crypto::toBase64("hello world, this is stefan");
    CHECK(encoded == "aGVsbG8gd29ybGQsIHRoaXMgaXMgc3RlZmFu");

    std::string decoded = bika::Crypto::fromBase64("aGVsbG8gd29ybGQsIHRoaXMgaXMgc3RlZmFu");
    CHECK(decoded == "hello world, this is stefan");
}

//---------------------------------------------------------------------------------------------------------------------
TEST_CASE("Hashing") {
    const std::string input = "hello world, this is stefan";

    CHECK(bika::Crypto::hash(input, bika::Crypto::sha256) == "a9e359b4ecc43d19af52b37328c810ff31e8ebe94c94790297ba024c22eb52fa");
    CHECK(bika::Crypto::hash(input, bika::Crypto::sha3)   == "cc6d66d8896ad136889a5ae109cecb929f7d6cf909bee5855870f4ee27c3eb6b");
    CHECK(bika::Crypto::hash(input, bika::Crypto::keccak) == "81005f88d5c38b3e8002d089f0653ac256a9ce62a9863bb9332fe739797a6742");
    CHECK(bika::Crypto::hash(input, bika::Crypto::sha1)   == "a371feb02f5a9ac48ea7b91456f4c196da859745");
    CHECK(bika::Crypto::hash(input, bika::Crypto::md5)    == "8132d7691c6e9ec6cd32dbf6fd51793f");
    CHECK(bika::Crypto::hash(input, bika::Crypto::cr32)   == "eba1b793");
}


} // Crypto