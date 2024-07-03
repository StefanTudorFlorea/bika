#pragma once
#include <any>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <yaml-cpp/yaml.h>


namespace bika {

class Config {
public:
    Config(const std::string& filename);

    // query sources
    Config& env(const std::string& name);
    Config& cfg(const std::string& path);

    // TODO: convert template const char* to std::string automatically
    template<typename T>
    Config& val(const T& val) {
        _val = val;
        return *this;
    }
    Config& val(const char* val) {
        _val = std::string{val};
        return *this;
    }

    template<typename T>
    operator T() {

        if (!_val.has_value()) {
            throw std::runtime_error("missing value");
        }

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
    bool try_convert(const std::string& str, int& result);
    bool try_convert(const std::string& str, bool& result);

    std::vector<std::string> split(const std::string& text, char delim) const;

private:
    std::optional<std::any> _val{};
    std::string _filename;
};

} // ns bika