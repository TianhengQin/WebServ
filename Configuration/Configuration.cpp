
#include "Configuration.hpp"

Configuration::Configuration() {
    Server s1;
    s1.setHost("127.0.0.1");
    s1.setPort(8080);
    s1.setServName("127.0.0.1");
    s1.setErrPage(301, "./websites/err_page/301.html");
    s1.setErrPage(404, "./websites/err_page/404.html");
    s1.setCliMaxBody(1024*1024);
    s1.setRoot("./websites");
    Location l1;
    l1.setPath("/");
    l1.setMethods(GET|PUT|DELETE);
    l1.setRoot("/");
    l1.setCgi(".php", "/usr/bin/php");
    l1.setCgi(".sh", "/bin/bash");
    l1.setCgi(".py", "/usr/local/bin/python3");
    Location l2;
    l2.setPath("/test");
    l2.setMethods(GET|PUT|DELETE);
    l2.setRoot("/test_page");
    l2.setDirListing(true);
    l2.setIndex("index.html");
    s1.setLocation(l1);
    s1.setLocation(l2);
    _servs.push_back(s1);

    Server s2;
    s2.setHost("127.0.0.1");
    s2.setPort(8080);
    s2.setServName("localhost");
    s2.setRoot("./websites");
    s2.setLocation(l1);
    _servs.push_back(s2);

    Server s3;
    s3.setHost("127.0.0.1");
    s3.setPort(8081);
    s3.setServName("localhost");
    s3.setRoot("./websites");
    s3.setLocation(l1);
    _servs.push_back(s3);
}

Configuration::Configuration(std::string filename) : _filename(filename) {
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
					// Server serv;
					// serv.setHost("127.0.0.1");
					// serv.setPort(8080);
					// serv.setServName("127.0.0.1");
					// serv.setErrPage(301, "./websites/err_page/301.html");
					// serv.setErrPage(404, "./websites/err_page/404.html");
					// serv.setCliMaxBody(1024*1024);
					// serv.setRoot("./websites");
					
					// this->_servs.push_back(server);
				}
			}
		}
	}
}

Configuration::~Configuration() {
	// delete _root;
}

std::vector<Server> &Configuration::getServs(void) {
	return this->_servs;
}
