#pragma once

#include <algorithm>
#include <cstdlib>
#include <fmt/core.h>
#include <iterator>
#include <optional>
#include <sstream>
#include <string_view>
#include <string>
#include <type_traits>
#include <yaml-cpp/yaml.h>

namespace bika {

class Config {
public:
    Config(std::string_view filename) 
        : _filename{filename} {
        // empty
    }

    template<typename T>
    T get(const std::string& path, std::optional<std::string> env = {}, std::optional<T> init = {}) {

        // detect local file changes
        _config = YAML::LoadFile(_filename);

        // set init as fallback option
        T value{};
        bool found = false;
        if (init) {
            value = init.value();
            found = true;
        }

        // search yaml
        YAML::Node node = _config;
        for (auto t : split(path, '.')) {
            node = node[t];
        }
        if (node.IsDefined()) {
            value = node.as<T>();
            found = true;
        }

        // search env var
        if(env) {
            if (char* envVar = std::getenv(env.value().c_str())) {

                // must convert manually to correct type
                if constexpr (std::is_integral_v<T>)          value = std::stoi(envVar);
                if constexpr (std::is_floating_point_v<T>)    value = std::stof(envVar);
                if constexpr (std::is_same_v<T, std::string>) value = std::string(envVar);

                found = true;
            }
        }

        // no default, no env, no static found
        if (!found) {
            throw std::runtime_error("value not found");
        }

        // done
        return value;
    }

private:
    // tokenize text based on delim
    std::vector<std::string> split(const std::string& text, char delim) {
        std::vector<std::string> tokens;
        std::istringstream iss{text};
        std::string item;
        
        while(std::getline(iss, item, delim)) {
            tokens.push_back(item);
        }
        return tokens;
    }

private:
    YAML::Node _config;
    const std::string _filename;
};

} // ns bika