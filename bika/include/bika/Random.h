#pragma once
#include <string>
#include <vector>


namespace bika {

/*  Basic random generation for numbers, strings and from a list of choices
    examples:
        - random between -2 and 7     => Random::value(-2,7)
        - random up to 10             => Random::value(10)
        - random name of 6 letters    => Random::text(6, "name-")
        - random currency from a list => Random::choice({"EUR", "USD", "RON"})
        - random uuid of 12 characters=> Random::text(12)
        - random true or false        => Random::choice({true, false})
*/
class Random {
public:
    // random between min...max. They can also be negative
    static int value(int min, int max);
    
    // random between 0...max
    static int value(int max);

    // random string of 0..9a..zA..Z
    static std::string uuid(int length = 6);

    // random string of count characters from the alphabet with optional prefix
    static std::string text(int count, const std::string& source = "abcdefghijklmnopqrstuvwxyz");

    // random from a vector of elements
    template<typename T>
    static auto choice(const std::vector<T>& values) {
        auto it = std::begin(values);
        return *(it + Random::value(values.size()-1));
    }

    // random from a list of elements
    template<typename T>
    static auto choice(const std::initializer_list<T>& values) {
        auto it = std::begin(values);
        return *(it + Random::value(values.size()-1));
    }
};

} // ns bika