
#include "Configuration.hpp"

Configuration::Configuration(void) {
    std::cout << "Configuration constructor (default)" << std::endl;
    this->_filename = "";
    // this->_servs = std::vector<Server>(1, Server());
}

Configuration::Configuration(std::string filename) : _filename(filename) {
    std::cout << "Configuration constructor using: " << filename << std::endl;
    
    // Read file content to a string
    std::ifstream file(this->_filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + this->_filename);
    }

    _root = parseConfig(file);
    return;

    // std::stringstream buffer;
    // buffer << file.rdbuf();
    // std::string fileContent = buffer.str();
    // file.close();
    // std::cout << "File content:\n" << fileContent << std::endl;

    // // Parse file content to map
    // std::string line;
    // std::stringstream ss(fileContent);
    // while (std::getline(ss, line, '\n')) {
    //     std::string key;
    //     std::string value;
    //     std::stringstream lineStream(line);
    //     std::getline(lineStream, key, '=');
    //     std::getline(lineStream, value, '=');
    //     this->_config[key] = value;
    // }    
}

Configuration::~Configuration() {
    std::cout << "Configuration destructor" << std::endl;
}

