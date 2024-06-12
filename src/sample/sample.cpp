#include <crow.h>
#include <cstdlib>
#include <fmt/core.h>
#include <pqxx/pqxx>
#include <string>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>


void test_config() {

    // static
    YAML::Node config = YAML::LoadFile("app.yml");
    int var1 = config["Config"]["var1"].as<int>();

    // dynamic
    if (const char* env = std::getenv("VAR1")) {
        var1 = std::stoi(env);
    }
}

void test_database() {
    pqxx::connection conn{fmt::format("host={} port={} dbname={} user={} password={}",
                                      "postgres", "5432", "data", "admin", "demo123")};

    // write
    {
        pqxx::work tx{conn};
        tx.exec("INSERT INTO Demo(name) VALUES('sfl')");
        tx.commit();
    }

    // read
    {
        pqxx::work tx{conn};
        auto[userId, name] = tx.query1<int, std::string>("SELECT userId,name FROM Demo WHERE id = 1");
    }
}

void test_rest() {
    crow::SimpleApp server;

    CROW_ROUTE(server, "/hello")([](){
        return "OK";
    });

    CROW_ROUTE(server, "/users/<int>").methods("POST"_method) ([](const crow::request& req, int uid) {

        // parse with better json lib
        nlohmann::json body = nlohmann::json::parse(req.body);
        nlohmann::json header = nlohmann::json::parse(req.body);

        // body params
        std::string content = body["content"];

        // header params
        std::string auth = req.get_header_value("Authorization");

        // query params
        std::string name = req.url_params.get("name");

        // code response
        return crow::response(crow::status::OK);
    });

    server.port(8080).run();
}

void test_redis() {
    // TODO
}

void test_rabbitmq() {
    // TODO
}

int main() {
    
}