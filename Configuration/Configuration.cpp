
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
	l1.setMethods(GET|POST|DELETE);
	l1.setCgi(".php", "/usr/bin/php");
	l1.setCgi(".sh", "/bin/bash");
	l1.setCgi(".py", "/usr/local/bin/python3");
	Location l2;
	l2.setPath("/test");
	l2.setMethods(GET|PUT|DELETE);
	l2.setRoot("/test_page");
	l2.setAutoindex(true);
	l2.setIndex("index.html");
	s1.setLocation(l1);
	s1.setLocation(l2);
	_servs.push_back(s1);

	Server s2;
	s2.setHost("127.0.0.1");
	s2.setPort(8080);
	s2.setServerName("localhost");
	s2.setRoot("./websites");
	s2.setLocation(l1);
	_servs.push_back(s2);

	Server s3;
	s3.setHost("127.0.0.1");
	s3.setPort(8081);
	s3.setServerName("localhost");
	s3.setRoot("./websites");
	s3.setLocation(l1);
	_servs.push_back(s3);
}

Configuration::Configuration(std::string filename) : _filename(filename) {
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + this->_filename);
	}
	parseConfig(file);
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
		this->_ast_root = other._ast_root;
		this->_servs = other._servs;
	}
	return (*this);
}

void	Configuration::parseConfig(std::ifstream &file) {
	NginxParser		parser(file);
	
	this->_ast_root = parser.getRoot();
	std::vector<ASTNode *> rootChildren = this->_ast_root->getChildren();

	for (std::vector<ASTNode *>::iterator child = rootChildren.begin(); child != rootChildren.end(); ++child) {
		Block *block = dynamic_cast<Block*>(*child);
		if (block && block->getName() == "http") {
			std::vector<ASTNode *> blockChildren = block->getChildren();
			for (std::vector<ASTNode *>::iterator http_child = blockChildren.begin(); http_child != blockChildren.end(); ++http_child) {
				Block *serverBlock = dynamic_cast<Block*>(*http_child);
				if (serverBlock && serverBlock->getName() == "server") {
					Server	server;
					this->_servs.push_back(server);
					setServerConfig(serverBlock, this->_servs.back());
				}
			}
		}
	}
}

void Configuration::setServerConfig(Block *block, Server &server) {
	std::vector<ASTNode *> blockChildren = block->getChildren();
	
	for (std::vector<ASTNode *>::iterator child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		Directive *directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			std::string name = directive->getName();
			std::vector<std::string> args = directive->getArguments();

			if (name == "listen") {
				processListenDirective(args, server);
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
				processLocationBlock(*child, server);
			}
		}
	}
}

void Configuration::processListenDirective(std::vector<std::string> &args, Server &server) {
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
	server.setDefault(isDefaultServer);
}

void Configuration::processLocationBlock(ASTNode *locationNode, Server &server) {
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

		if (name == "root") {
			loc.setRoot(args[0]);
		} else if (name == "index") {
			loc.setIndex(args[0]);
		} else if (name == "limit_except") {
			loc.setMethods((int)parseMethods(args));
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


std::string Configuration::getFilename(void) {
	return (this->_filename);
}

Block *Configuration::getASTRoot(void) {
	return (this->_ast_root);
}

std::vector<Server> Configuration::getServs(void) {
	return (this->_servs);
}


std::ostream &operator<<(std::ostream &out, Configuration &config) {
	out << "Configuration file: " << config.getFilename() << std::endl;
	out << "Servers: " << std::endl;
	std::vector<Server> servs = config.getServs();
	for (std::vector<Server>::iterator serv = servs.begin(); serv != servs.end(); ++serv) {
		out << *serv << std::endl;
	}
	return (out);
}
