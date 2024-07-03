#pragma once
#include <string>
#include <vector>

namespace bika {

class Crypto {
public:
    // base64 encode/decode
    static std::string toBase64(const std::string &in);
    static std::string fromBase64(const std::string &in);
};

} // ns bika