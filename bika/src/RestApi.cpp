#include "bika/RestApi.h"
#include <fmt/core.h>
#include <regex>
#include <unordered_set>
#include "bika/RestApi.h"


namespace bika {

//---------------------------------------------------------------------------------------------------------------------
void RestApi::POST(const std::string& path, handler_t handler) {
    _app.Post(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::GET(const std::string& path, handler_t handler) {
    _app.Get(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::PUT(const std::string& path, handler_t handler) {
    _app.Put(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::DELETE(const std::string& path, handler_t handler) {
    _app.Delete(convertPath(path), [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler) {
    enableCors(res);

    // process handler request,response
    const Request r{json::parse(req.body), req.headers, req.params, req.path_params};
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
void RestApi::start(const std::string &host, int port)
{
    _app.listen(host, port);
}

//---------------------------------------------------------------------------------------------------------------------
void RestApi::enableCors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "*");
    res.set_header("Access-Control-Allow-Methods", "*");
}

} // ns bika