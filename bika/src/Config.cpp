#include "bika/Config.h"


namespace bika {

Config::Config(const std::string& filename)
    : _filename{filename} {
    // empty
}

//---------------------------------------------------------------------------------------------------------------------
Config& Config::env(const std::string& name) {
    if (char* envVar = std::getenv(name.c_str())) {
        _val = std::string{envVar};

        if (int intVar; try_convert(envVar, intVar)) {
            _val = intVar;
        }

        if (bool boolVar; try_convert(envVar, boolVar)) {
            _val = boolVar;
        }
        
        // TODO: add support for float
    }
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
Config& Config::cfg(const std::string& path) {

    // TODO: add support for json
    YAML::Node node = YAML::LoadFile(_filename);
    for (auto t : split(path, '.')) {
        node = node[t];
    }

    if (node.IsDefined()) {
        _val = node;
    }
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
bool Config::try_convert(const std::string& str, int& result) {
    std::istringstream iss(str);
    iss >> std::noskipws;
    if (iss >> result && iss.eof()) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
bool Config::try_convert(const std::string& str, bool& result) {
    std::istringstream iss(str);
    if (iss >> std::boolalpha >> result) {
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
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