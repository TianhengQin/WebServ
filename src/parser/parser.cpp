
#include "parser.hpp"

Parser::Parser(const std::string& filename) : filename(filename) {}

Parser::~Parser() {}

Parser::parse() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, ':')) {
            std::getline(iss, value);
            data[trim(key)] = trim(value);
        }
    }
}


Parser::printData() const {
    for (auto& it : data) {
        std::cout << it.first << " : " << it.second << std::endl;
    }
}
