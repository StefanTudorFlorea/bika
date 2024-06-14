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

/*  Load configuration settings from a yaml file and override them with environment variables if necessary

    Precedence:
        1. (optional) load yaml settings
        2. (optional) load env var
        3. (optional) set defaults if no value found

    Supported types: int, string, bool

    example: Load from yaml, but if env var is specified, overwrite the result
        Config cfg{"settings.yml"};
        int port = cfg.get<int>("configuration.ports.web", "CUSTOM_PORT");

    example: Just load directly from yaml
        Config cfg{"settings.yml"};
        int port = cfg.get<int>("configuration.ports.web");

*/
class Config {
public:
    Config(std::string_view filename);

    template<typename T>
    T get(std::optional<std::string> path, std::optional<std::string> env = {}, std::optional<T> init = {}) const {

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
        if (path) {
            YAML::Node node = _config;
            for (auto t : split(path.value(), '.')) {
                node = node[t];
            }
            if (node.IsDefined()) {
                value = node.as<T>();
                found = true;
            }
        }

        // search env var
        if(env) {
            if (char* envVar = std::getenv(env.value().c_str())) {
                value = convert<T>(envVar);
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

    // convert from string to different types
    template<typename T>
    T convert(const std::string& str) const {

        if constexpr (std::is_integral_v<T>)          return std::stoi(str);
        if constexpr (std::is_floating_point_v<T>)    return std::stof(str);
        if constexpr (std::is_same_v<T, std::string>) return std::string(str);

        throw std::runtime_error("Unsupported type for conversion");
    }

    // tokenize text based on delim
    std::vector<std::string> split(const std::string& text, char delim) const;

private:
    mutable YAML::Node _config;
    const std::string _filename;
};

} // ns bika