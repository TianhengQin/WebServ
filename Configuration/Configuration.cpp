
#include "Configuration.hpp"

/**
 * @brief Construct a new Configuration:: Configuration object
 * 
 */

Configuration::Configuration(void) { }


Configuration::Configuration(const Configuration &other) {
	*this = other;
}

Configuration &Configuration::operator=(const Configuration &other) {
	if (this != &other) {
		this->_filename = other._filename;
		this->_parser = other._parser;
		this->_root = other._root;
		this->_index = other._index;
		this->_error_page = other._error_page;
		this->_client_max_body_size = other._client_max_body_size;
		this->_allow_methods = other._allow_methods;
		this->_autoindex = other._autoindex;
		this->_servers = other._servers;
		this->_cgi = other._cgi;
	}
	return (*this);
}

Configuration::~Configuration() {
	delete this->_parser;
}


Configuration::Configuration(std::string filename) : _filename(filename) {
	this->_root = "";
	this->_index = "";
	this->_error_page = std::map<int, std::string>();
	this->_client_max_body_size = 1024 * 1024;
	this->_allow_methods = GET | POST | DELETE | PUT | HEAD;
	this->_autoindex = false;

	// this->_parser = NginxParser(filename);
	this->parse_configuration_file();
}

void	Configuration::parse_configuration_file(void) {
	Block			*httpBlock;
	std::ifstream	file(this->_filename.c_str());

	if (!file.is_open()) {
		throw std::runtime_error("Error opening file: " + this->_filename);
	}
	this->_parser = new NginxParser(file);
	httpBlock = this->_parser->getHttpBlock();
	// std::cout << *httpBlock << std::endl; // Debug

	try {
		this->process_http_block(httpBlock);
	} catch (std::exception &e) {
		throw std::runtime_error("\n Error processing configuration file: " + this->_filename + "\n " + e.what());
	}
}

void Configuration::process_http_block(Block *httpBlock) {
	std::vector<ASTNode *>				blockChildren;
	std::vector<ASTNode *>::iterator	child;
	Block 								*serverBlock;
	Directive							*directive;
	std::string							name;
	std::vector<std::string>			args;

	blockChildren = httpBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (!directive)
			continue;
		name = directive->getName();
		args = directive->getArguments();
		if (name == "root") {
			if (_parser->is_absolute_path(args[0]))
				this->_root = args[0];
			else
				throw std::runtime_error("Invalid root path: " + args[0]);
		} else if (name == "index") {
			for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
				if (_parser->is_absolute_path(*it))
					this->_index = *it;
				else
					throw std::runtime_error("Invalid index path: " + *it);
			}
		} else if (name == "error_page") {
			if (args.size() == 2) {
				if (_parser->is_absolute_path(args[1]))
					this->_error_page[std::stoi(args[0])] = args[1];
				else
					throw std::runtime_error("Invalid error page path: " + args[1]);
			} else {
				throw std::runtime_error("Insufficient arguments for error_page directive");
			}
		} else if (name == "client_max_body_size") {
			this->_client_max_body_size = parseSize(args[0]);
		} else if (name == "allow_methods") { // || name == "limit_except") {
			this->_allow_methods = parseMethods(args);
		} else if (name == "autoindex") {
			if (args[0] == "on") {
				this->_autoindex = true;
			} else if (args[0] == "off") {
				this->_autoindex = false;
			}
		}
	}
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		serverBlock = dynamic_cast<Block*>(*child);
		if (serverBlock && serverBlock->getName() == "server") {
			Server serv;

			set_server_context(serv);
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
	std::string							name;
	std::vector<std::string>			args;

	blockChildren = serverBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			name = directive->getName();
			args = directive->getArguments();
			if (args.empty())
				continue;
			else if (name == "listen") {
				process_listen_directive(args, server);
			} else if (name == "server_name") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					server.addServerName(*it);
				}
			} else if (name == "root") {
				if (_parser->is_absolute_path(args[0]))
					server.setRoot(args[0]);
				else
					throw std::runtime_error("Invalid root path :" + args[0]);
			} else if (name == "index") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					if (_parser->is_absolute_path(*it))
						server.addIndex(*it);
					else
						throw std::runtime_error("Invalid index path :" + *it);
				}
			} else if (name == "allow_methods") {
				server.setAllowedMethods(parseMethods(args));
			} else if (name == "error_page") {
				if (args.size() == 2) {
					if (_parser->is_absolute_path(args[1]))
						server.setErrorPage(std::stoi(args[0]), args[1]);
					else
						throw std::runtime_error("Invalid error page path" + args[1]);
				} else {
					throw std::runtime_error("Insufficient arguments for error_page directive");
				}
			} else if (name == "client_max_body_size") {
				server.setClientMaxBodySize(parseSize(args[0]));
			} else if (name == "autoindex") {
				if (args[0] == "on") {
					server.setAutoindex(true);
				} else if (args[0] == "off") {
					server.setAutoindex(false);
				}
			} else if (name == "cgi") {
				try {
					if (args.size() >= 2) {
						if (_parser->is_absolute_path(args[1]))
							server.setCgi(args[0], args[1]);
						else
							throw std::runtime_error("Invalid cgi path" + args[1]);
					} else {
						throw std::runtime_error("Insufficient arguments for cgi directive");
					}
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
			args = locationBlock->getArguments();
			if (!args.empty() && _parser->is_absolute_path(args[0])) {
				loc.setPath(args[0]);
			} else {
				throw std::runtime_error("Invalid location path");
			}
			process_location_block(locationBlock, loc);
			server.addLocation(loc);
		}
	}
}



void Configuration::process_location_block(Block *locationBlock, Location &location) {
	std::vector<ASTNode *>				blockChildren;
	std::vector<ASTNode *>::iterator	child;
	Directive							*directive;
	std::string							name;
	std::vector<std::string>			args;

	blockChildren = locationBlock->getChildren();
	for (child = blockChildren.begin(); child != blockChildren.end(); ++child) {
		directive = dynamic_cast<Directive*>(*child);
		if (directive) {
			name = directive->getName();
			args = directive->getArguments();
			if (args.empty()) {
				continue;
			} else if (name == "root") {
				if (_parser->is_absolute_path(args[0]))
					location.setRoot(args[0]);
				else
					throw std::runtime_error("Invalid root path:" + args[0]);
			} else if (name == "alias") {
				if (_parser->is_absolute_path(args[0]) && args[0].back() == '/')
					location.setAlias(args[0]);
				else
					throw std::runtime_error("Invalid alias path:" + args[0]);
			} else if (name == "index") {
				for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
					if (_parser->is_absolute_path(*it))
						location.addIndex(*it);
					else
						throw std::runtime_error("Invalid index path");
				}
			} else if (name == "error_page") {
				if (args.size() == 2) {
					if (_parser->is_absolute_path(args[1]))
						location.setErrorPage(std::stoi(args[0]), args[1]);
					else
						throw std::runtime_error("Invalid error page path:" + args[1]);
				} else {
					throw std::runtime_error("Insufficient arguments for error_page directive");
				}
			} else if (name == "allow_methods"){ //} || name == "limit_except") {
				location.setAllowedMethods(parseMethods(args));
			} else if (name == "client_max_body_size") {
				location.setClientMaxBodySize(parseSize(args[0]));
			} else if (name == "return") {
				if (_parser->is_absolute_path(args[0]))
					location.setRedir(args[0]);
				else
					throw std::runtime_error("Invalid return path:" + args[0]);
			} else if (name == "autoindex") {
				if (args[0] == "on") {
					location.setAutoindex(true);
				} else if (args[0] == "off") {
					location.setAutoindex(false);
				}
			} else if (name == "cgi") {
				if (args.size() == 2) {
					if (_parser->is_absolute_path(args[1]))
						location.setCgi(args[0], args[1]);
					else
						throw std::runtime_error("Invalid cgi path:" + args[1]);
				} else {
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
	std::string		host = "0.0.0.0";
	int				port = 80;
	bool			isDefaultServer = false;

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
	(void)isDefaultServer;
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

std::vector<Server> Configuration::getServers(void) { return (this->_servers); }



/* Operator overloads */

std::ostream &operator<<(std::ostream &out, Configuration &config) {
	out << "Configuration file: " << config.getFilename() << std::endl;
	out << "Root: " << config.getRoot() << std::endl;
	out << "Index: " << config.getIndex() << std::endl;
	out << "Error Pages: " << std::endl;
	std::map<int, std::string> errorPages = config.getErrorPages();
	for (std::map<int, std::string>::iterator it = errorPages.begin(); it != errorPages.end(); ++it) {
		out << "  " << it->first << " " << it->second << std::endl;
	}
	out << "Client Max Body Size: " << config.getClientMaxBodySize() << std::endl;
	out << "Allowed Methods: " << config.getAllowedMethods() << std::endl;
	out << "Autoindex: " << config.getAutoindex() << std::endl;
	out << "CGI: " << std::endl;
	out << "Servers: " << std::endl;
	std::vector<Server> servs = config.getServers();
	for (std::vector<Server>::iterator serv = servs.begin(); serv != servs.end(); ++serv) {
		out << *serv << std::endl;
	}
	return (out);
}
