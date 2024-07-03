#pragma once
#include <string>
#include <vector>
#include <sha256.h>
#include <md5.h>
#include <crc32.h>
#include <sha3.h>
#include <sha1.h>
#include <keccak.h>


namespace bika {

class Crypto {
public:
    // base64 encode/decode
    static std::string toBase64(const std::string &in);
    static std::string fromBase64(const std::string &in);

    // hash
    enum Hash { sha256, md5, cr32, sha3, sha1, keccak};
    static std::string hash(const std::string& in, Hash h = Hash::sha256);
};

} // ns bika