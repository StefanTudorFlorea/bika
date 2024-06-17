#include "bika/RestApi.h"
#include <fmt/core.h>
#include <unordered_set>


namespace bika {


void RestApi::POST(const std::string& path, handler_t handler) {
    _app.Post(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::GET(const std::string& path, handler_t handler) {
    _app.Get(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::PUT(const std::string& path, handler_t handler) {
    _app.Put(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::DELETE(const std::string& path, handler_t handler) {
    _app.Delete(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::handleApiCalls(const httplib::Request& req, httplib::Response& res, handler_t handler) {
    cors(res);

    // process handler request,response
    const Request r{req.body, req.params, req.headers};
    Response out = handler(r);

    // prepare response
    res.status = out.status;
    res.body = out.body;
    res.set_content(res.body, "application/json");
}

void RestApi::start(const std::string& host, int port) {
    _app.listen(host, port);
}

void RestApi::cors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
}

} // ns bika