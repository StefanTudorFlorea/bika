#pragma once
#include <functional>
#include <httplib.h>
#include <string>
#include <nlohmann/json.hpp>


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

    See: https://github.com/yhirose/cpp-httplib
*/
class RestApi {
public:
    struct Request { 
        json body;        // sent with POST and we convert it to json
        json headers;     // usually for auth => Access headers["Authorization"]
        json queryParams; // ?name=stefan&age=42 => Access queryParams["name"], queryParams["age"] ALL string
        json pathParams;  // /users/:id, /users/:name => Access pathParams["id"], pathParams["name"] ALL string
    };
    struct Response { 
        int status; 
        json body{}; 
    };
    using handler_t = std::function<Response(Request)>;

public:
    // server routes
    void POST(const std::string& path,   handler_t handler);
    void GET(const std::string& path,    handler_t handler);
    void PUT(const std::string& path,    handler_t handler);
    void DELETE(const std::string& path, handler_t handler);

    // run the server and listen to requests
    void start(const std::string& host, int port);

private:
    // set global cors headers
    void enableCors(httplib::Response& res);

    // handle api requests
    void handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler);

    // convert from {id} to :id in the path
    std::string convertPath(const std::string& origPath);

private:
    httplib::Server _app;
};

} // ns bika