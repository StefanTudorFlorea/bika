#pragma once
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <optional>

// acceptable to have project wide rename
using json = nlohmann::json;


namespace bika {

/*  Http client that allows to make post,get,put,delete requests to an endpoint
    Accepts optional params, headers and body
    Status is 0 if URL not reachable

    usage: 
        auto[status, text] = bika::Http::GET("http://www.httpbin.org/get", json{{"name", "stefan"}, {"age", 42}});
*/
class HttpClient {
public:
    struct Response {
        int status = 200;
        std::string body = "";
    };

    static Response POST(const std::string& url,   const json& params={}, const json& headers={}, const std::string& body={});
    static Response GET(const std::string& url,    const json& params={}, const json& headers={}, const std::string& body={});
    static Response PUT(const std::string& url,    const json& params={}, const json& headers={}, const std::string& body={});
    static Response DELETE(const std::string& url, const json& params={}, const json& headers={}, const std::string& body={});

private:
    // conversion
    static cpr::Parameters toCprParams(const json& params);
    static cpr::Header toCprHeader(const json& params);
};

} // ns bika