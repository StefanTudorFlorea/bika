#pragma once

#include <functional>
#include <string>
#include <httplib.h>
#include <nlohmann/json.hpp>

// acceptable to have project wide rename
using json = nlohmann::json;

namespace bika {

/*  Create a simple rest api server with cors enabled

    Supported http methods: GET, POST, DELETE, PUT
    Callbacks can either return a simple string or json

    example: Add a GET /users
        RestApi api;
        api.add("GET", "/users", [](auto req, auto& res) {
            return "OK";
        });

    See: https://github.com/yhirose/cpp-httplib
*/
class RestApi {
    using handler_t = std::function<nlohmann::json(httplib::Request, httplib::Response&)>;

public:
    // add a rest api route
    void add(std::string_view method, const std::string& path, handler_t handler);

    // start the server
    void start(const std::string& host, int port);

private:
    // set global cors headers
    void cors(httplib::Response& res);
    
private:
    httplib::Server _app;
};

} // ns bika