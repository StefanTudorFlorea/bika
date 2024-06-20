#include "bika/Random.h"
#include <random>


namespace bika {

int Random::value(int min, int max) {
    static std::mt19937 e{std::random_device{}()};
    std::uniform_int_distribution<int> uniform_dist(min, max);

    return uniform_dist(e);
}

int Random::value(int max) {
    static std::mt19937 e{std::random_device{}()};
    std::uniform_int_distribution<int> uniform_dist(0, max);

    return uniform_dist(e);
}

std::string Random::text(int count, const std::string& prefix) {
    static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

    std::string result{prefix};
    for (int i = 0; i < count; ++i) {
        int randIdx = Random::value(0, alphabet.size()-1);
        result += alphabet.at(randIdx);
    }

    return result;
}

} // ns bika