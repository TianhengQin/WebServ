
#include <iostream>

int main() {
    auto http = std::make_shared<Block>("http", std::vector<std::string>{});

    http->addChild(std::make_shared<Directive>("access_log", std::vector<std::string>{"logs/access.log"}));
    http->addChild(std::make_shared<Directive>("error_log", std::vector<std::string>{"logs/error.log", "warn"}));

    auto server = std::make_shared<Block>("server", std::vector<std::string>{});
    server->addChild(std::make_shared<Directive>("listen", std::vector<std::string>{"80"}));
    server->addChild(std::make_shared<Directive>("server_name", std::vector<std::string>{"localhost"}));

    auto location = std::make_shared<Block>("location", std::vector<std::string>{"/"});
    location->addChild(std::make_shared<Directive>("proxy_pass", std::vector<std::string>{"http://localhost:8080"}));
    location->addChild(std::make_shared<Directive>("index", std::vector<std::string>{"index.html", "index.htm"}));

    server->addChild(location);
    http->addChild(server);
    http->print();

    return 0;
}

