#pragma once
#include <nlohmann/json.hpp>
#include <httplib.h>


namespace handler {

inline nlohmann::json ping(const httplib::Request& req) {
    return {
        {"error", false},
        {"message", "OK"}
    };
}

} // ns handler