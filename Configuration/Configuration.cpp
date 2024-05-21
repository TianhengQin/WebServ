
#include "Configuration.hpp"

Configuration::Configuration(void) {
	Server s1;
	s1.setHost("127.0.0.1");
	s1.setPort(8080);
	s1.setServerName("127.0.0.1");
	s1.setErrorPage(301, "./websites/err_page/301.html");
	s1.setErrorPage(404, "./websites/err_page/404.html");
	s1.setClientMaxBodySize(1024*1024);
	s1.setRoot("./websites");
	Location l1;
	l1.setPath("/");
	l1.setRoot("/");
	l1.setAllowedMethods(GET|POST|DELETE);
	l1.setCgi(".php", "/usr/bin/php");
	l1.setCgi(".sh", "/bin/bash");
	l1.setCgi(".py", "/usr/local/bin/python3");
	Location l2;
	l2.setPath("/test");
	l2.setAllowedMethods(GET|PUT|DELETE);
	l2.setRoot("/test_page");
	l2.setAutoindex(true);
	l2.setIndex("index.html");
	s1.setLocation(l1);
	s1.setLocation(l2);
	_servers.push_back(s1);

	Server s2;
	s2.setHost("127.0.0.1");
	s2.setPort(8080);
	s2.setServerName("localhost");
	s2.setRoot("./websites");
	s2.setLocation(l1);
	_servers.push_back(s2);

	Server s3;
	s3.setHost("127.0.0.1");
	s3.setPort(8081);
	s3.setServerName("localhost");
	s3.setRoot("./websites");
	s3.setLocation(l1);
	_servers.push_back(s3);
}

Configuration::Configuration(std::string filename) : _filename(filename) {
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + this->_filename);
	}
	parse_configuration_file(file);
}

Configuration::~Configuration() {
	// delete _root;
}

Configuration::Configuration(const Configuration &other) {
	*this = other;
}

Configuration &Configuration::operator=(const Configuration &other) {
	if (this != &other) {
		this->_filename = other._filename;
		this->_ast = other._ast;
		this->_servers = other._servers;
	}
	return (*this);
}

void	Configuration::parse_configuration_file(std::ifstream &file) {
	NginxParser		parser(file);
	
	this->_ast = parser.getRoot();
	std::vector<ASTNode *> rootChildren = this->_ast->getChildren();

	for (std::vector<ASTNode *>::iterator child = rootChildren.begin(); child != rootChildren.end(); ++child) {
		Block *block = dynamic_cast<Block*>(*child);
		if (block && block->getName() == "http") {
			process_http_block(block);
		}
	}
}

void Configuration::process_http_block(Block *httpBlock) {
	std::vector<ASTNode *> blockChildren = httpBlock->getChildren();
	
	for (std::vector<ASTNode *>::iterator child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		Block *serverBlock = dynamic_cast<Block*>(*child);
		if (serverBlock && serverBlock->getName() == "server") {
			this->_servers.push_back(Server());
			process_server_block(serverBlock, this->_servers.back());
		}
	}
}

void Configuration::process_server_block(Block *block, Server &server) {
	std::vector<ASTNode *> blockChildren = block->getChildren();
	
	for (std::vector<ASTNode *>::iterator child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		Directive *directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			std::string name = directive->getName();
			std::vector<std::string> args = directive->getArguments();

			if (name == "listen") {
				process_listen_directive(args, server);
			} else if (name == "server_name") {
				server.setServerName(args[0]);
			} else if (name == "root") {
				server.setRoot(args[0]);
			} else if (name == "error_page") {
				if (args.size() == 2) {
					server.setErrorPage(std::stoi(args[0]), args[1]);
				} else if (args.size() == 1) {
					server.setErrorPage(std::stoi(args[0]), "");
				}
			} else if (name == "client_max_body_size") {
				server.setClientMaxBodySize(parseSize(args[0]));
			}
		} else {
			Block *childBlock = dynamic_cast<Block*>(*child);
			if (childBlock && childBlock->getName() == "location") {
				process_location_block(*child, server);
			}
		}
	}
}


void Configuration::process_location_block(ASTNode *locationNode, Server &server) {
	Block *locationBlock = dynamic_cast<Block*>(locationNode);
	if (!locationBlock) {
		return;
		// throw std::runtime_error("Error casting location block");
	}
	
	std::vector<std::string> pathArgs = locationBlock->getArguments();
	std::vector<ASTNode *> locationChildren = locationBlock->getChildren();

	Location loc;
	loc.setPath(pathArgs[0]);

	for (std::vector<ASTNode *>::iterator locChild = locationChildren.begin(); locChild != locationChildren.end(); ++locChild) {
		Directive *directive = dynamic_cast<Directive*>(*locChild);
		if (!directive) {
			continue;
			// throw std::runtime_error("Error casting directive");
		}

		std::string name = directive->getName();
		std::vector<std::string> args = directive->getArguments();

		if (name == "cgi") {
			loc.setCgi(args[0], args[1]);
		} else if (name == "root") {
			loc.setRoot(args[0]);
		} else if (name == "index") {
			loc.setIndex(args[0]);
		} else if (name == "limit_except") {
			loc.setAllowedMethods((int)parseMethods(args));
		} else if (name == "autoindex") {
			if (args[0] == "on") {
				loc.setAutoindex(true);
			} else {
				loc.setAutoindex(false);
			}
		} 
		/*
		// TODO: cgi
			# CGI configurations
			fastcgi_pass 127.0.0.1:9000;
			fastcgi_index index.php;
			fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
			include fastcgi_params;
		*/
		// else if 
		// 	try {
		// 		loc.setCgi(args[0], args[1]);
		// 	} catch (std::exception &e) {
		// 		continue;
		// 		// throw std::runtime_error("Insufficient arguments for cgi directive");
		// 	}
		// } 
	}

	server.setLocation(loc);
}


void Configuration::process_listen_directive(std::vector<std::string> &args, Server &server) {
	std::string	host = "0.0.0.0";
	int			port = 80;
	bool		isDefaultServer = false;

	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
		if (*it == "default_server") {
			isDefaultServer = true;
		} else if (it->find(":") != std::string::npos) {
			size_t colonPos = it->find(":");
			host = it->substr(0, colonPos);
			port = std::stoi(it->substr(colonPos + 1));
		} else {
			port = std::stoi(*it);
		}
	}

	server.setHost(host);
	server.setPort(port);
}

int Configuration::parseSize(std::string sizeStr) {
	size_t numEnd = 0;
	while (numEnd < sizeStr.size() && std::isdigit(sizeStr[numEnd])) {
		numEnd++;
	}

	int size = 1024 * 1024;
	try {
		size = std::stoi(sizeStr.substr(0, numEnd));
	} catch (std::exception &e) {
		size = 1024 * 1024;
	}

	if (numEnd < sizeStr.size()) {
		char suffix = sizeStr[numEnd];
		switch (suffix) {
			case 'K':
			case 'k':
				size *= 1024;
				break;
			case 'M':
			case 'm':
				size *= 1024 * 1024;
				break;
			case 'G':
			case 'g':
				size *= 1024 * 1024 * 1024;
				break;
			default:
				size = 1024 * 1024;
		}
	}

	return size;
}

unsigned int Configuration::parseMethods(std::vector<std::string> &methods) {
	unsigned int methodFlags = 0;
	for (std::vector<std::string>::iterator method = methods.begin(); method != methods.end(); ++method) {
		std::transform(method->begin(), method->end(), method->begin(), ::toupper);

		if (*method == "GET") {
			methodFlags |= GET;
		} else if (*method == "POST") {
			methodFlags |= POST;
		} else if (*method == "PUT") {
			methodFlags |= PUT;
		} else if (*method == "DELETE") {
			methodFlags |= DELETE;
		}
	}
	return (methodFlags);
}

/* Getters */

std::string Configuration::getFilename(void) {
	return (this->_filename);
}

Block *Configuration::getASTRoot(void) {
	return (this->_ast);
}

std::vector<Server> Configuration::getServers(void) {
	return (this->_servers);
}

std::string Configuration::getRoot(void) { return (this->_root); }
std::string Configuration::getIndex(void) { return (this->_index); }
std::map<int, std::string> Configuration::getErrorPages(void) { return (this->_error_page); }
unsigned int Configuration::getClientMaxBodySize(void) { return (this->_client_max_body_size); }
unsigned int Configuration::getAllowedMethods(void) { return (this->_allow_methods); }
bool Configuration::getAutoindex(void) { return (this->_autoindex); }
std::map<std::string, std::string> Configuration::getCgi(void) { return (this->_cgi); }


/* Operator overloads */

std::ostream &operator<<(std::ostream &out, Configuration &config) {
	out << "Configuration file: " << config.getFilename() << std::endl;
	out << "Servers: " << std::endl;
	std::vector<Server> servs = config.getServers();
	for (std::vector<Server>::iterator serv = servs.begin(); serv != servs.end(); ++serv) {
		out << *serv << std::endl;
	}
	return (out);
}
