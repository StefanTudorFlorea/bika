#include "bika/RestApi.h"
#include <fmt/core.h>
#include <regex>
#include <unordered_set>
#include "bika/RestApi.h"


namespace bika {

//---------------------------------------------------------------------------------------------------------------------
void RestApi::POST(const std::string& path, handler_t handler) {
    _server.Post(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::GET(const std::string& path, handler_t handler) {
    _server.Get(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::PUT(const std::string& path, handler_t handler) {
    _server.Put(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::DELETE(const std::string& path, handler_t handler) {
    _server.Delete(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler) {
    enableCors(res);

    // process handler request,response
    // BUG: cannot parse body if empty
    Request r;
    r.body           = json::parse(req.body);
    r.headers        = req.headers;
    r.queryParams    = req.params;
    r.pathParams     = req.path_params;
    r.context.method = req.method;
    r.context.path   = req.path;

    Response out = handler(r);

    // prepare response
    res.status = out.status;
    res.body   = out.body.dump();
    res.set_content(res.body, "application/json");
}

//---------------------------------------------------------------------------------------------------------------------
std::string RestApi::convertPath(const std::string &origPath) {
    std::regex re(R"(\{([^\}]*)\})");
    return std::regex_replace(origPath, re, ":$1");
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::start(const std::string &host, int port) {
    _server.listen(host, port);
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::enableCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_header("Access-Control-Allow-Methods", "*");
}

} // ns bika