
#include "Configuration.hpp"

Config::Config(void) : _servs(std::vector<Server>(1, Server())) {
    std::cout << "Config constructor (default)" << std::endl;
}

Config::Config(std::string filename) {
    std::cout << "Config constructor using " << filename << std::endl;
    // TODO: parse the file and fill _servs
    
}

Config::~Config() {
    std::cout << "Config destructor" << std::endl;
}

