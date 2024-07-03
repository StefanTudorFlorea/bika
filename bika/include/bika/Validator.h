#pragma once
#include "Config.h"
#include <nlohmann/json-schema.hpp>
#include <string>


namespace bika {
class Validator {
public:
    Validator(const std::string& configFile) :
        _config{configFile} {
        // emtpy
    }

    void validate(const nlohmann::json& obj, const std::string& schemaPath) {
        nlohmann::json_schema::json_validator validator;
        validator.set_root_schema(nlohmann::json::parse(std::string{_config.cfg(schemaPath)}));
        validator.validate(obj);
    }
private:
    // nlohmann::json _schema;
    bika::Config _config;
};

} // ns bika