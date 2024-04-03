
#include "Configuration.hpp"

Configuration::Configuration(void) {
    std::cout << "Configuration constructor (default)" << std::endl;
    this->_filename = "";
    // this->_servs = std::vector<Server>(1, Server());
}

Configuration::Configuration(std::string filename) {
    std::cout << "Configuration constructor using " << filename << std::endl;
    // TODO: parse the file and fill _servs
    
}

Configuration::~Configuration() {
    std::cout << "Configuration destructor" << std::endl;
}

