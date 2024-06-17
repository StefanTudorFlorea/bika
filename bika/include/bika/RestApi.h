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

    example: Add a GET /ping
        RestApi api;
        api.GET("/ping", [](auto req) -> bika::RestApi::Response {
            return {200, "OK"};
        });
        api.start("0.0.0.0", 8080);

    example: client that makes a post request with query params
        auto[status, text] = bika::RestApi::GET("http://www.httpbin.org/get", json{{"hello", "world"}}, "body sample text");

    See: https://github.com/yhirose/cpp-httplib
    See: https://github.com/libcpr/cpr
*/
class RestApi {
public:
    using handler_t = std::function<nlohmann::json(httplib::Request, httplib::Response&)>;

    struct Request {
        std::string body;
        json headers;
        json params;
    };
    struct Response {
        int status       = 200;
        std::string body = "";
    };
    using handler2_t = std::function<Response(Request)>;

public:
    // server routes
    void POST(const std::string& path, handler2_t handler);
    void GET(const std::string& path, handler2_t handler);
    void PUT(const std::string& path, handler2_t handler);
    void DELETE(const std::string& path, handler2_t handler);

    // run the server and listen to requests
    void start(const std::string& host, int port);

    // client
    static Response POST(const std::string& url, const json& params={}, const std::string& body={});
    static Response GET(const std::string& url, const json& params={}, const std::string& body={});
    static Response PUT(const std::string& url, const json& params={}, const std::string& body={});
    static Response DELETE(const std::string& url, const json& params={}, const std::string& body={});

private:
    // set global cors headers
    void cors(httplib::Response& res);

    // handle api requests
    void handleApiCalls(const httplib::Request& req, httplib::Response& res, handler2_t handler);

    // conversion
    static cpr::Parameters fromJson(const json& params);

private:
    httplib::Server _app;
};

} // ns bika