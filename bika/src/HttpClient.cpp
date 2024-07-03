#include "bika/HttpClient.h"

// conversion helpers
namespace {
cpr::Header toCprHeader(const json& params) {
    cpr::Header header;
    for (const auto& el : params.items()) {
        header.emplace(el.key(), el.value());
    }
    return header;
}

cpr::Parameters toCprParams(const json& params) {
    cpr::Parameters p;
    for (const auto& el : params.items()) {
        p.Add(cpr::Parameter{el.key(), el.value()});
    }
    return p;
}
}

namespace bika {

//---------------------------------------------------------------------------------------------------------------------
HttpClient::Response HttpClient::POST(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Post(cpr::Url{url}, toCprParams(params), toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

//---------------------------------------------------------------------------------------------------------------------
HttpClient::Response HttpClient::GET(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Get(cpr::Url{url}, toCprParams(params), toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

//---------------------------------------------------------------------------------------------------------------------
HttpClient::Response HttpClient::PUT(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Put(cpr::Url{url}, toCprParams(params), toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

//---------------------------------------------------------------------------------------------------------------------
HttpClient::Response HttpClient::DELETE(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Delete(cpr::Url{url}, toCprParams(params), toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

} // ns bika