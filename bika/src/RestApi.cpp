#include "bika/RestApi.h"


namespace bika {

void RestApi::start(const std::string& host, int port) {
    _app.listen(host, port);
}

void RestApi::add(std::string_view method, const std::string& path, handler_t handler) {

    // wrapper for actual handler
    auto callback = [this](handler_t handler, const httplib::Request& req, httplib::Response& res){
        cors(res);
        nlohmann::json out = handler(req, res);
        res.set_content(out.dump(), "application/json");
    };

    if (method == "GET") {
        _app.Get(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }

    if (method == "POST") {
        _app.Post(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }

    if (method == "DELETE") {
        _app.Delete(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }

    if (method == "PUT") {
        _app.Put(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }
}

void RestApi::cors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
}

} // ns bika