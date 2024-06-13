#include "bika/Config.h"

namespace bika {

Config::Config(std::string_view filename) : _filename{filename} {
    // empty
}

std::vector<std::string> Config::split(const std::string& text, char delim) const {

    std::vector<std::string> tokens;
    std::istringstream iss{text};
    std::string item;
    
    while(std::getline(iss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

} // ns bika