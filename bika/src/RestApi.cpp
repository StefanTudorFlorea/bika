#include "bika/RestApi.h"
#include <fmt/core.h>
#include <cassert>
#include <unordered_set>

namespace bika {

RestApi::Response RestApi::POST(const std::string& url, const json& params, const std::string& body) {
    cpr::Response r = cpr::Post(cpr::Url{url}, RestApi::fromJson(params), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

RestApi::Response RestApi::GET(const std::string& url, const json& params, const std::string& body) {
    cpr::Response r = cpr::Get(cpr::Url{url}, RestApi::fromJson(params), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

RestApi::Response RestApi::PUT(const std::string& url, const json& params, const std::string& body) {
    cpr::Response r = cpr::Put(cpr::Url{url}, RestApi::fromJson(params), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

RestApi::Response RestApi::DELETE(const std::string& url, const json& params, const std::string& body) {
    cpr::Response r = cpr::Delete(cpr::Url{url}, RestApi::fromJson(params), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

void RestApi::POST(const std::string& path, handler2_t handler) {
    _app.Post(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::GET(const std::string& path, handler2_t handler) {
    _app.Get(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::PUT(const std::string& path, handler2_t handler) {
    _app.Put(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::DELETE(const std::string& path, handler2_t handler) {
    _app.Delete(path, [this,handler](const httplib::Request& req, httplib::Response& res) {
        handleApiCalls(req, res, handler);
    });
}

void RestApi::handleApiCalls(const httplib::Request& req, httplib::Response& res, handler2_t handler) {
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

cpr::Parameters RestApi::fromJson(const json& params) {
    cpr::Parameters p;

    for (const auto& el : params.items()) {
        p.Add(cpr::Parameter{el.key(), el.value()});
    }

    return p;
}

} // ns bika