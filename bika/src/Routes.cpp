#include "bika/Routes.h"


Routes::Routes(httplib::Server& app) : _app{app} {
    // empty
}

void Routes::add(std::string_view method, const std::string& path, handler_t handler) {

    if (method == "GET") {
        _app.Get(path, [this, handler](const httplib::Request& req, httplib::Response& res) {
            cors(res);
            nlohmann::json out = handler(req);
            res.set_content(out.dump(), "application/json");
        });
    }

    if (method == "POST") {
        _app.Post(path, [this, handler](const httplib::Request& req, httplib::Response& res) {
            cors(res);
            nlohmann::json out = handler(req);
            res.set_content(out.dump(), "application/json");
        });
    }

    if (method == "DELETE") {
        _app.Delete(path, [this, handler](const httplib::Request& req, httplib::Response& res) {
            cors(res);
            nlohmann::json out = handler(req);
            res.set_content(out.dump(), "application/json");
        });
    }

    if (method == "PUT") {
        _app.Put(path, [this, handler](const httplib::Request& req, httplib::Response& res) {
            cors(res);
            nlohmann::json out = handler(req);
            res.set_content(out.dump(), "application/json");
        });
    }

    if (method == "OPTIONS") {
        _app.Options(path, [this, handler](const httplib::Request& req, httplib::Response& res) {
            cors(res);
            nlohmann::json out = handler(req);
            res.set_content(out.dump(), "application/json");
        });
    }
}

void Routes::cors(httplib::Response& res) {

    res.set_header("Access-Control-Allow-Origin",      "*");
    res.set_header("Access-Control-Allow-Methods",     "GET, POST, DELETE, PUT, OPTIONS");
    res.set_header("Access-Control-Allow-Credentials", "true");
    res.set_header("Access-Control-Allow-Headers",     "Accept, Authorization, Content-Type");
}