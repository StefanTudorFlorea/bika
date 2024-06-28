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

    // TODO: get values from optional redis store
    Config2& store(const std::string& key) {
        return *this;
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
            
            // TODO: add support for float
        }
        return *this;
    }

    Config2& cfg(const std::string& path) {

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

    // TODO: convert template const char* to std::string automatically
    template<typename T>
    Config2& val(const T& val) {
        _val = val;
        return *this;
    }
    Config2& val(const char* val) {
        _val = std::string{val};
        return *this;
    }

    template<typename T>
    operator T() {

        try {
            auto val = std::any_cast<YAML::Node>(_val.value());
            return val.as<T>();
        } catch (const std::bad_any_cast) {
            // nothing, as we could still have another type below
        }

        return std::any_cast<T>(_val.value());
    }

private:
    // REFACTOR: add one template for both with constexpr if for the type
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
    std::optional<std::any> _val{};
    std::string _filename;
};