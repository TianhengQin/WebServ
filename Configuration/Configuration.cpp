
#include "Configuration.hpp"

Configuration::Configuration(void) {
	std::cout << "Configuration constructor (default)" << std::endl;
	this->_filename = "";
	// this->_servs = std::vector<Server>(1, Server());
}

Configuration::Configuration(std::string filename) : _filename(filename)
{
	std::cout << "Configuration constructor using: " << filename << std::endl;
	
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + this->_filename);
	}

	_root = parseConfig(file);
	
	// std::vector<ASTNode *>	server_blocks = _root->getServers();
	for (ASTNode *child : _root->getChildren()) {
		if (child->getName() == "http") {
			Block *block = dynamic_cast<Block*>(child);
			if (!block) {
				throw std::runtime_error("Error casting block");
			}
			for (ASTNode *http_child : block->getChildren()) {
				if (http_child->getName() == "server") {
					http_child->print(); // debug
					// Server *server = new Server();

					// TODO: implement setup method
					// server->setup(http_child); 
					
					// this->_servs.push_back(*server);
					Server cur;
					
				}
			}
		}
	}
}

Configuration::~Configuration() {
	std::cout << "Configuration destructor" << std::endl;
}

std::vector<Server> &Configuration::getServs(void) {
	return this->_servs;
}