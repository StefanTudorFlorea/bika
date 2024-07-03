#include "bika/Crypto.h"


namespace bika {

//---------------------------------------------------------------------------------------------------------------------
std::string Crypto::toBase64(const std::string &in) {

    const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    
    int val = 0;
    int bits = -6;

    for (unsigned char c : in) {
        val = (val << 8) + c;
        bits += 8;

        while (bits >= 0) {
            out.push_back(base64_chars[(val >> bits) & 0x3F]);
            bits -= 6;
        }
    }

    if (bits > -6) {
        out.push_back(base64_chars[((val << 8) >> (bits + 8)) & 0x3F]);
    }

    while (out.size() % 4) {
        out.push_back('=');
    }

    return out;
}

//---------------------------------------------------------------------------------------------------------------------
std::string Crypto::fromBase64(const std::string &in) {

    const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<int> decoding_table(256, -1);
    
    // Initialize decoding table
    for (int i = 0; i < 64; ++i) {
        decoding_table[base64_chars[i]] = i;
    }

    std::string out;
    int val = 0;
    int bits = -8;

    for (unsigned char c : in) {
        if (decoding_table[c] == -1) break; // Ignore invalid characters
        
        val = (val << 6) + decoding_table[c];
        bits += 6;
        
        if (bits >= 0) {
            out.push_back(static_cast<char>((val >> bits) & 0xFF));
            bits -= 8;
        }
    }

    return out;
}

//---------------------------------------------------------------------------------------------------------------------
std::string Crypto::hash(const std::string& in, Hash h) {
    switch (h) {
        case sha256: return SHA256{}(in);
        case md5:    return MD5{}(in);
        case cr32:   return CRC32{}(in);
        case sha1:   return SHA1{}(in);
        case sha3:   return SHA3{}(in);
        case keccak: return Keccak{}(in);
    }

    return in;
}

} // ns bika