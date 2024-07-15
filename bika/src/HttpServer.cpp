#include "bika/HttpServer.h"
#include <fmt/core.h>
#include <regex>
#include <unordered_set>


namespace bika {

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::setPreHandler(handler_t handler) {

    _server.set_pre_routing_handler([this, handler](const auto& req, auto& res) {

        // prepare request for the handler
        Request r;
        r.body           = req.body.empty() ? json{} : json::parse(req.body);
        r.headers        = req.headers;
        r.queryParams    = req.params;
        r.pathParams     = req.path_params;
        r.context.method = req.method;
        r.context.path   = req.path;

        const json out = handler(r);

        // returned an error, dont process further routes
        if (out.at("status") != 200) {

            // prepare response
            res.status = out["status"];
            res.body   = out["body"].dump();
            res.set_content(res.body, "application/json");

            return httplib::Server::HandlerResponse::Handled;
        }

        return httplib::Server::HandlerResponse::Unhandled;
    });
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::POST(const std::string& path, handler_t handler) {
    _server.Post(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::GET(const std::string& path, handler_t handler) {
    _server.Get(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::PUT(const std::string& path, handler_t handler) {
    _server.Put(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::DELETE(const std::string& path, handler_t handler) {
    _server.Delete(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler) {
    enableCors(res);

    // process handler request,response
    Request r;
    r.body           = req.body.empty() ? json{} : json::parse(req.body);
    r.headers        = req.headers;
    r.queryParams    = req.params;
    r.pathParams     = req.path_params;
    r.context.method = req.method;
    r.context.path   = req.path;

    const json& out = handler(r);

    // prepare response
    res.status  = out.value("status", 200); // implicit OK, if not defined
    res.body    = out.at("body").dump();    // 'body' must be defined

    // override default headers if user set some
    if (out.contains("headers")) {
        for (const auto h: out.at("headers")) {
            const auto [headerName, headerValue] = parseHttHeader(h);
            res.set_header(headerName, headerValue);
        }
    }
    res.set_content(res.body, "application/json");
}

//---------------------------------------------------------------------------------------------------------------------
std::pair<std::string, std::string> HttpServer::parseHttHeader(const std::string& header) {
    
    if (auto pos = header.find(':'); pos != std::string::npos) {
        std::string name = header.substr(0, pos);
        std::string value = header.substr(pos+1);
        return {name, value};
    }

    return {"", ""};
}

//---------------------------------------------------------------------------------------------------------------------
std::string HttpServer::convertPath(const std::string &origPath) {
    std::regex re(R"(\{([^\}]*)\})");
    return std::regex_replace(origPath, re, ":$1");
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::start(const std::string &host, int port) {
    _server.listen(host, port);
}

//---------------------------------------------------------------------------------------------------------------------
void HttpServer::enableCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_header("Access-Control-Allow-Methods", "*");
}

} // ns bika