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

        // additional information about the call
        struct Context {
            std::string method;
            std::string path;
        };
        Context context;
    };
    struct Response { 
        int status; 
        json body{}; 
    };
    using handler_t = std::function<Response(Request)>;

public:
    // runt before handlers
    void setPreHandler(handler_t handler) {

        _server.set_pre_routing_handler([this, handler](const auto& req, auto& res) {

            Request r;
            r.body           = json::parse(req.body);
            r.headers        = req.headers;
            r.queryParams    = req.params;
            r.pathParams     = req.path_params;
            r.context.method = req.method;
            r.context.path   = req.path;

            const Response out = handler(r);

            // returned an error, dont process further routes
            if (out.status != 200) {

                // prepare response
                res.status = out.status;
                res.body   = out.body.dump();
                res.set_content(res.body, "application/json");

                return httplib::Server::HandlerResponse::Handled;
            }

            return httplib::Server::HandlerResponse::Unhandled;
        });
    }

    // server routes
    void POST(const std::string& path,   handler_t handler);
    void GET(const std::string& path,    handler_t handler);
    void PUT(const std::string& path,    handler_t handler);
    void DELETE(const std::string& path, handler_t handler);

    // run the server and listen to requests
    void start(const std::string& host, int port);

    httplib::Server& server() {
        return _server;
    }

private:
    // set global cors headers
    void enableCors(httplib::Response& res);

    // handle api requests
    void handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler);

    // convert from {id} to :id in the path
    std::string convertPath(const std::string& origPath);

private:
    httplib::Server _server;
};

} // ns bika