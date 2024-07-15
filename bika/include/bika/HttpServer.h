#pragma once
#include <functional>
#include <httplib.h>
#include <string>
#include <nlohmann/json.hpp>
#include <utility>


// acceptable to have project wide rename
using json = nlohmann::json;

namespace bika {

/*  Create a simple rest api server with cors enabled
    Or a simple client that calls rest api
    Supported http methods: GET, POST, DELETE, PUT
    Response is of type json and has the following optional fields:
    - "status": INT. Default = 200 if not set
    - "body": JSON. Default = {} if not set
    - "headers": STRING LIST. Default = {} if not set

    example: Add a GET /ping
        RestApi api;
        api.GET("/ping", [](auto req) -> json {
            return {
                {"status", 200},
                {"body", "this is my body"}
            }
        });
        api.start("0.0.0.0", 8080);

    See: https://github.com/yhirose/cpp-httplib
*/
class HttpServer {
public:
    struct Request { 
        json body;        // sent with POST and we convert it to json
        json headers;     // usually for auth => Access headers["Authorization"]
        json queryParams; // ?name=stefan&age=42 => Access queryParams["name"], queryParams["age"] ALL string
        json pathParams;  // /users/{id}, /users/{name} => Access pathParams["id"], pathParams["name"] ALL string

        // additional information about the call
        struct Context {
            std::string method;
            std::string path;
        };
        Context context;
    };
    
    using handler_t = std::function<json(Request)>;

public:
    // runt before handlers
    void setPreHandler(handler_t handler);

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

    // from a header of form `My-Header: some random value' extra header name and value
    std::pair<std::string, std::string> parseHttHeader(const std::string& header);

private:
    httplib::Server _server;
};

} // ns bika