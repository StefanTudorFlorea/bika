#include "bika/Http.h"


namespace bika {

Http::Response Http::POST(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Post(cpr::Url{url}, Http::toCprParams(params), Http::toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

Http::Response Http::GET(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Get(cpr::Url{url}, Http::toCprParams(params), Http::toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

Http::Response Http::PUT(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Put(cpr::Url{url}, Http::toCprParams(params), Http::toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

Http::Response Http::DELETE(const std::string& url, const json& params, const json& headers, const std::string& body) {
    cpr::Response r = cpr::Delete(cpr::Url{url}, Http::toCprParams(params), Http::toCprHeader(headers), cpr::Body{body});
    return {static_cast<int>(r.status_code), r.text};
}

cpr::Header Http::toCprHeader(const json& params) {
    cpr::Header header;
    
    for (const auto& el : params.items()) {
        header.emplace(el.key(), el.value());
    }

    return header;
}


cpr::Parameters Http::toCprParams(const json& params) {
    cpr::Parameters p;

    for (const auto& el : params.items()) {
        p.Add(cpr::Parameter{el.key(), el.value()});
    }

    return p;
}

} // ns bika