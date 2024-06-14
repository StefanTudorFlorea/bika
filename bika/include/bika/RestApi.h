#pragma once

#include <cpr/cpr.h>
#include <functional>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <utility>


// acceptable to have project wide rename
using json = nlohmann::json;

namespace bika {

/*  Create a simple rest api server with cors enabled
    Or a simple client that calls rest api

    Supported http methods: GET, POST, DELETE, PUT
    Callbacks can either return a simple string or json

    example: Add a GET /users
        RestApi api;
        api.add("GET", "/users", [](auto req, auto& res) {
            return "OK";
        });

    example: client that makes a post request with query params
     auto[status, text] = bika::RestApi::call("POST", 
                                              "http://www.httpbin.org/get", 
                                              "body sample text",
                                              cpr::Parameters{{"hello", "world"}});

    See: https://github.com/yhirose/cpp-httplib
    See: https://github.com/libcpr/cpr
*/
class RestApi {
public:
    using handler_t = std::function<nlohmann::json(httplib::Request, httplib::Response&)>;

public:
    // add a rest api route
    void add(const std::string& method, const std::string& path, handler_t handler);

    // start the server
    void start(const std::string& host, int port);

    // client
    static std::tuple<int, std::string> call(const std::string& method,
                                             const std::string& url,
                                             const std::string& body = {},
                                             const cpr::Parameters& query = {});
private:
    // set global cors headers
    void cors(httplib::Response& res);

private:
    httplib::Server _app;
};

} // ns bika