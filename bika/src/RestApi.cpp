#include "bika/RestApi.h"
#include <fmt/core.h>
#include <cassert>
#include <unordered_set>

namespace check {
void httpMethod(const std::string& method) {
    static std::unordered_set<std::string> allowedMethod{"GET", "POST", "PUT", "DELETE"};

    assert(allowedMethod.contains(method));
    if (!allowedMethod.contains(method)) {
        throw std::runtime_error("Unsupported http method");
    }
}
} // ns check


namespace bika {

std::tuple<int, std::string> RestApi::call(const std::string& method, 
                                           const std::string& url, 
                                           const std::string& body,
                                           const cpr::Parameters& query) {
    check::httpMethod(method);

    // get a response
    cpr::Response r{};
    if (method == "GET") {
        r = cpr::Get(cpr::Url{url}, query, cpr::Body{body});
    }
    if (method == "POST") {
        r = cpr::Post(cpr::Url{url}, query, cpr::Body{body});
    }
    if (method == "PUT") {
        r = cpr::Put(cpr::Url{url}, query, cpr::Body{body});
    }
    if (method == "DELETE") {
        r = cpr::Delete(cpr::Url{url}, query, cpr::Body{body});
    }

    // done
    return {r.status_code, r.text};
}


void RestApi::start(const std::string& host, int port) {
    _app.listen(host, port);
}

void RestApi::add(const std::string& method, const std::string& path, handler_t handler) {
    check::httpMethod(method);

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

    if (method == "PUT") {
        _app.Put(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }

    if (method == "DELETE") {
        _app.Delete(path, [=](const httplib::Request& req, httplib::Response& res) {
            callback(handler, req, res);
        });
    }
}

void RestApi::cors(httplib::Response& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
}

} // ns bika