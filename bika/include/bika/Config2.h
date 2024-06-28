#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <optional>
#include <variant>
#include <yaml-cpp/yaml.h>
#include <any>
#include <sstream>

class Config2 {
public:
    // set sources
    void setFile(const std::string& filename) {
        _filename = filename;
    }

    // query sources
    Config2& env(const std::string& name) {
        if (char* envVar = std::getenv(name.c_str())) {
            _val = std::string{envVar};

            if (int intVar; try_convert(envVar, intVar)) {
                _val = intVar;
            }

            if (bool boolVar; try_convert(envVar, boolVar)) {
                _val = boolVar;
            }
        }
        return *this;
    }

    Config2& cfg(const std::string& path) {

        YAML::Node node = YAML::LoadFile(_filename);
        for (auto t : split(path, '.')) {
            node = node[t];
        }

        if (node.IsDefined()) {
            _val = node;
        }
        return *this;
    }

    template<typename T>
    Config2& val(const T& val) {
        _val = val;
        return *this;
    }

    template<typename T>
    operator T() {

        if (auto val = std::get_if<YAML::Node>(&_val.value())) {
            return val->as<T>();
        }
        
        if (auto val = std::get_if<T>(&_val.value())) {
            return *val;
        }

        return {};
    }

private:
    bool try_convert(const std::string& str, int& result) {
        std::istringstream iss(str);
        iss >> std::noskipws;
        if (iss >> result && iss.eof()) {
            return true;
        }
        return false;
    }

    bool try_convert(const std::string& str, bool& result) {
        std::istringstream iss(str);
        if (iss >> std::boolalpha >> result) {
            return true;
        }
        return false;
    }

    std::vector<std::string> split(const std::string& text, char delim) const {
        std::vector<std::string> tokens;
        std::istringstream iss{text};
        std::string item;
        
        while(std::getline(iss, item, delim)) {
            tokens.push_back(item);
        }
        
        return tokens;
    }

private:
    std::optional<std::variant<
        float, 
        bool, 
        int, 
        std::string, 
        YAML::Node,
        >> _val{};
    std::string _filename;
};