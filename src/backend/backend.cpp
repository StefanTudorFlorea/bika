#include "common/Routes.h"
#include "common/Config.h"
#include "handler.h"
#include <fmt/core.h>
#include <httplib.h>
#include <yaml-cpp/yaml.h>


int main() {

    // YAML::Node config = YAML::LoadFile("backend.yml");
    // int port = config["Configuration"]["webPort"].as<int>();

    Config config{"backend.yml"};

    auto port = config.get<int>("Configuration.ports.webPort", {}, 42);
    fmt::println("port {}", port);
}