#pragma once
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <functional>


class Routes {
    using handler_t = std::function<nlohmann::json(httplib::Request)>;

public:
    Routes(httplib::Server& app);

    void add(std::string_view method, const std::string& path, handler_t handler);

private:
    void cors(httplib::Response& res);
    
private:
    httplib::Server& _app;
};