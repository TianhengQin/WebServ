
#include "Configuration.hpp"

/**
 * @brief Construct a new Configuration:: Configuration object
 * 
 */

Configuration::Configuration(void) {
	Server s1;
	set_server_context(s1);
	s1.setHost("127.0.0.1");
	s1.setPort(8080);
	s1.addServerName("127.0.0.1");
	s1.setErrorPage(301, "./websites/err_page/301.html");
	s1.setErrorPage(404, "./websites/err_page/404.html");
	s1.setClientMaxBodySize(1024*1024);
	s1.setRoot("./websites");
	
	Location l1;
	set_location_context(s1, l1);
	l1.setPath("/");
	l1.setRoot("/HTML");
	l1.setAllowedMethods(GET|POST|DELETE);
	l1.setRedir("/HTML/index.html");
	l1.setCgi(".php", "/usr/bin/php");
	l1.setCgi(".sh", "/bin/bash");
	l1.setCgi(".py", "/usr/local/bin/python3");
	Location l2;
	set_location_context(s1, l2);
	l2.setPath("/test");
	l2.setAllowedMethods(GET|PUT|DELETE);
	l2.setRoot("/HTML/test_page");
	l2.setAutoindex(true);
	l2.addIndex("/HTML/index.html");
	s1.addLocation(l1);
	s1.addLocation(l2);
	_servers.push_back(s1);

	Server s2;
	set_server_context(s2);
	s2.setHost("127.0.0.1");
	s2.setPort(8080);
	s2.addServerName("localhost");
	s2.setRoot("./websites");
	set_location_context(s2, l1);
	s2.addLocation(l1);
	_servers.push_back(s2);

	Server s3;
	set_server_context(s3);
	s3.setHost("127.0.0.1");
	s3.setPort(8081);
	s3.addServerName("localhost");
	s3.setRoot("./websites");
	set_location_context(s3, l1);
	s3.addLocation(l1);
	_servers.push_back(s3);
}


Configuration::Configuration(const Configuration &other) {
	*this = other;
}

Configuration &Configuration::operator=(const Configuration &other) {
	if (this != &other) {
		// Todo: deep copy
	}
	return (*this);
}

Configuration::~Configuration() {
	// delete _root;
}


Configuration::Configuration(std::string filename) : _filename(filename) {
	this->_root = "";
	this->_index = "";
	this->_error_page = std::map<int, std::string>();
	this->_client_max_body_size = 1024 * 1024;
	this->_allow_methods = GET | POST | DELETE | PUT | HEAD;
	this->_autoindex = false;
	// this->_parser = NginxParser(filename);

	// this->_parser.parse_configuration_file();
	parse_configuration_file();
	// process_http_block(this->_parser.getHttpBlock());
}

// void	Configuration::parse_configuration_file(std::ifstream &file) {
void	Configuration::parse_configuration_file(void) {
	std::ifstream file(this->_filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + this->_filename);
	}

	this->_parser = new NginxParser(file);
	Block *httpBlock = this->_parser->getHttpBlock();
	// std::cout << *httpBlock << std::endl;
	process_http_block(httpBlock);

}

void Configuration::process_http_block(Block *httpBlock) {
	std::vector<ASTNode *>				blockChildren;
	std::vector<ASTNode *>::iterator	child;
	Block 								*serverBlock;
	Directive							*directive;
	std::vector<std::string>			args;

	blockChildren = httpBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (!directive)
			continue;
		args = directive->getArguments();
		if (directive->getName() == "root") {
			this->_root = args[0];
		} else if (directive->getName() == "index") {
			this->_index = args[0];
		} else if (directive->getName() == "error_page") {
			if (args.size() == 2) {
				this->_error_page[std::stoi(args[0])] = args[1];
			} else if (args.size() == 1) {
				this->_error_page[std::stoi(args[0])] = "";
			}
		} else if (directive->getName() == "client_max_body_size") {
			this->_client_max_body_size = parseSize(args[0]);
		// } else if (directive->getName() == "limit_except") { // Real `limit_except` directive is in location block
		} else if (directive->getName() == "allow_methods") {
			this->_allow_methods = parseMethods(args);
		} else if (directive->getName() == "autoindex") {
			if (args[0] == "on") {
				this->_autoindex = true;
			} else {
				this->_autoindex = false;
			}
		}
	}
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		serverBlock = dynamic_cast<Block*>(*child);
		if (serverBlock && serverBlock->getName() == "server") {
			Server serv;
			set_server_context(serv);
			// serv.setRoot(this->_root);
			// serv.addIndex(this->_index);
			// serv.setAllowedMethods(this->_allow_methods);
			// for (std::map<int, std::string>::iterator it = this->_error_page.begin(); it != this->_error_page.end(); ++it) {
			// 	serv.setErrorPage(it->first, it->second);
			// }
			// serv.setClientMaxBodySize(this->_client_max_body_size);
			// serv.setAutoindex(this->_autoindex);
			// for (std::map<std::string, std::string>::iterator it = this->_cgi.begin(); it != this->_cgi.end(); ++it) {
			// 	serv.setCgi(it->first, it->second);
			// }

			process_server_block(serverBlock, serv);
			this->_servers.push_back(serv);
		}
	}
}

void Configuration::process_server_block(Block *serverBlock, Server &server) {
	std::vector<ASTNode *>				blockChildren;
	std::vector<ASTNode *>::iterator	child;
	Block								*locationBlock;
	Directive							*directive;
	std::vector<std::string>			args;

	blockChildren = serverBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			args = directive->getArguments();
			if (directive->getName() == "listen") {
				process_listen_directive(args, server);
			} else if (directive->getName() == "server_name") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					server.addServerName(*it);
				}
				// server.addServerName(args[0]);
			} else if (directive->getName() == "root") {
				server.setRoot(args[0]);
			} else if (directive->getName() == "index") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					server.addIndex(*it);
				}
				// server.addIndex(args[0]);
			} else if (directive->getName() == "allow_methods") {
				server.setAllowedMethods(parseMethods(args));
			} else if (directive->getName() == "error_page") {
				if (args.size() == 2) {
					server.setErrorPage(std::stoi(args[0]), args[1]);
				} else if (args.size() == 1) {
					server.setErrorPage(std::stoi(args[0]), "");
				}
			} else if (directive->getName() == "client_max_body_size") {
				server.setClientMaxBodySize(parseSize(args[0]));
			} else if (directive->getName() == "autoindex") {
				if (args[0] == "on") {
					server.setAutoindex(true);
				} else if (args[0] == "off") {
					server.setAutoindex(false);
				}
			} else if (directive->getName() == "cgi") {
				try {
					server.setCgi(args[0], args[1]);
				} catch (std::exception &e) {
					throw std::runtime_error("Insufficient arguments for cgi directive");
				}
			}
		}
	}
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		locationBlock = dynamic_cast<Block*>(*child);
		if (locationBlock && locationBlock->getName() == "location") {
			Location loc;
			set_location_context(server, loc);
			loc.setPath(locationBlock->getArguments()[0]);
			// loc.setRoot(server.getRoot());
			// loc.addIndex(server.getIndex());
			// loc.setAllowedMethods(server.getAllowedMethods());
			// loc.setAutoindex(server.getAutoindex());
			// loc.setClientMaxBodySize(server.getClientMaxBodySize());
			// std::map<int, std::string> errorPages = server.getErrorPages();
			// for (std::map<int, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
			// 	loc.setErrorPage(it->first, it->second);
			// }
			// std::map<std::string, std::string> cgi = server.getCgi();
			// for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); ++it) {
			// 	loc.setCgi(it->first, it->second);
			// }
			process_location_block(locationBlock, loc);
			server.addLocation(loc);
		}
	}
}



void Configuration::process_location_block(Block *locationBlock, Location &location) {
	std::vector<ASTNode *>				blockChildren;
	std::vector<ASTNode *>::iterator	child;
	Directive							*directive;
	std::vector<std::string>			args;

	blockChildren = locationBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			args = directive->getArguments();
			if (directive->getName() == "root") {
				location.setRoot(args[0]);
			} else if (directive->getName() == "index") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					location.addIndex(*it);
				}
			} else if (directive->getName() == "error_page") {
				if (args.size() == 2) {
					location.setErrorPage(std::stoi(args[0]), args[1]);
				} else if (args.size() == 1) {
					location.setErrorPage(std::stoi(args[0]), "");
				}
			} else if (directive->getName() == "allow_methods"){ //} || directive->getName() == "limit_except") {
				location.setAllowedMethods(parseMethods(args));
			} else if (directive->getName() == "client_max_body_size") {
				location.setClientMaxBodySize(parseSize(args[0]));
			} else if (directive->getName() == "return") {
				location.setRedir(args[0]);
			} else if (directive->getName() == "autoindex") {
				if (args[0] == "on") {
					location.setAutoindex(true);
				} else if (args[0] == "off") {
					location.setAutoindex(false);
				}
			} else if (directive->getName() == "cgi") {
				try {
					location.setCgi(args[0], args[1]);
				} catch (std::exception &e) {
					throw std::runtime_error("Insufficient arguments for cgi directive");
				}
			}
		}
	}
}

void Configuration::set_server_context(Server &server) {
	server.setRoot(this->_root);
	server.addIndex(this->_index);
	server.setAllowedMethods(this->_allow_methods);
	for (std::map<int, std::string>::iterator it = this->_error_page.begin(); it != this->_error_page.end(); ++it) {
		server.setErrorPage(it->first, it->second);
	}
	server.setClientMaxBodySize(this->_client_max_body_size);
	server.setAutoindex(this->_autoindex);
	for (std::map<std::string, std::string>::iterator it = this->_cgi.begin(); it != this->_cgi.end(); ++it) {
		server.setCgi(it->first, it->second);
	}
}

void Configuration::set_location_context(Server &server, Location &location) {
	location.setRoot(server.getRoot());
	location.addIndex(server.getIndex());
	location.setAllowedMethods(server.getAllowedMethods());
	location.setAutoindex(server.getAutoindex());
	location.setClientMaxBodySize(server.getClientMaxBodySize());
	std::map<int, std::string> errorPages = server.getErrorPages();
	for (std::map<int, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
		location.setErrorPage(it->first, it->second);
	}
	std::map<std::string, std::string> cgi = server.getCgi();
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); ++it) {
		location.setCgi(it->first, it->second);
	}
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



unsigned int Configuration::parseSize(std::string sizeStr) {
	size_t numEnd = 0;
	while (numEnd < sizeStr.size() && std::isdigit(sizeStr[numEnd])) {
		numEnd++;
	}

	unsigned int size = 1;
	try {
		size = std::stoi(sizeStr.substr(0, numEnd));
	} catch (std::exception &e) {
		size = 1;
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
		} else if (*method == "DELETE") {
			methodFlags |= DELETE;
		} else if (*method == "PUT") {
			methodFlags |= PUT;
		} else if (*method == "HEAD") {
			methodFlags |= HEAD;
		}
	}
	return (methodFlags);
}

/* Getters */


std::string Configuration::getFilename(void) { return (this->_filename); }

NginxParser *Configuration::getParser(void) { return (this->_parser); }

std::string Configuration::getRoot(void) { return (this->_root); }

std::string Configuration::getIndex(void) { return (this->_index); }

std::map<int, std::string> Configuration::getErrorPages(void) { return (this->_error_page); }

unsigned int Configuration::getClientMaxBodySize(void) { return (this->_client_max_body_size); }

unsigned int Configuration::getAllowedMethods(void) { return (this->_allow_methods); }

bool Configuration::getAutoindex(void) { return (this->_autoindex); }

std::map<std::string, std::string> Configuration::getCgi(void) { return (this->_cgi); }

std::vector<Server> Configuration::getServers(void) {
	return (this->_servers);
}


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
