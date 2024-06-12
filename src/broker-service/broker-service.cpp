#include "common/Routes.h"
#include "handler.h"
#include <fmt/core.h>
#include <httplib.h>
#include <yaml-cpp/yaml.h>


int main() {

    // config
    YAML::Node config = YAML::LoadFile("config.yml");
    int port = config["BrokerService"]["port"].as<int>();

    // server
    httplib::Server app;
    Routes routes{app};

    // routes   
    routes.add("GET", "/ping", handler::ping);

    // start
    fmt::println("[broker-service]: listening on port {}", port);
    app.listen("0.0.0.0", port);
}