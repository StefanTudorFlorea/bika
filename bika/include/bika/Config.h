#pragma once

#include <algorithm>
#include <cstdlib>
#include <fmt/core.h>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <yaml-cpp/yaml.h>

namespace bika {

class Config {
public:
    Config(const std::string& filename) {
        _config = YAML::LoadFile(filename);
    }

    template<typename T>
    T get(const std::string& path, std::optional<std::string> env = {}, std::optional<T> init = {}) {

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
            if (char* env_var = std::getenv(env.value().c_str())) {
                if constexpr (std::is_integral_v<T>)          value = std::stoi(env_var);
                if constexpr (std::is_floating_point_v<T>)    value = std::stof(env_var);
                if constexpr (std::is_same_v<T, std::string>) value = std::string(env_var);

                found = true;
            }
        }

        if (!found) {
            throw std::runtime_error("value not found");
        }

        // done
        return value;
    }

private:
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
};

} // ns bika