
#include "Configuration.hpp"

Configuration::Configuration(void) {
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

	this->_root = parseConfig(file);
	
	std::vector<ASTNode *> rootChildren = this->_root->getChildren();
	for (std::vector<ASTNode *>::iterator child = rootChildren.begin(); child != rootChildren.end(); ++child) {
		Block *block = dynamic_cast<Block*>(*child);

		if (block && block->getName() == "http") {
			std::vector<ASTNode *> blockChildren = block->getChildren();
			for (std::vector<ASTNode *>::iterator http_child = blockChildren.begin(); http_child != blockChildren.end(); ++http_child) {
				// if ((*http_child)->getName() == "server") {
				// 	// (*http_child)->print(); // debug
				// 	Server server;
				// 	this->_servs.push_back(server);
				// 	setServerConfig(dynamic_cast<Block*>(*http_child), this->_servs.back());
				// }
				Block *serverBlock = dynamic_cast<Block*>(*http_child);
				if (serverBlock && serverBlock->getName() == "server") {
					Server server;
					this->_servs.push_back(server);
					setServerConfig(serverBlock, this->_servs.back());
				}
			}
		}
	}
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
		this->_root = other._root;
		this->_servs = other._servs;
	}
	return (*this);
}

Block *Configuration::parseConfig(std::ifstream &file) {
	NginxParser parser(file);
	return parser.getRoot();
}

// void Configuration::setServerConfig(Block *block, Server &server) {
// 	std::vector<ASTNode *> blockChildren = block->getChildren();
// 	for (std::vector<ASTNode *>::iterator child = blockChildren.begin(); child != blockChildren.end(); ++child) {
// 		if ((*child)->getName() == "listen") {
// 			Directive *directive = dynamic_cast<Directive*>(*child);
// 			if (!directive) {
// 				throw std::runtime_error("Error casting directive");
// 			}
// 			server.setHost(directive->getArguments()[0]);
// 			server.setPort(std::stoi(directive->getArguments()[1]));
// 		} else if ((*child)->getName() == "server_name") {
// 			Directive *directive = dynamic_cast<Directive*>(*child);
// 			if (!directive) {
// 				throw std::runtime_error("Error casting directive");
// 			}
// 			server.setServName(directive->getArguments()[0]);
// 		} else if ((*child)->getName() == "root") {
// 			Directive *directive = dynamic_cast<Directive*>(*child);
// 			if (!directive) {
// 				throw std::runtime_error("Error casting directive");
// 			}
// 			server.setRoot(directive->getArguments()[0]);
// 		} else if ((*child)->getName() == "error_page") {
// 			Directive *directive = dynamic_cast<Directive*>(*child);
// 			if (!directive) {
// 				throw std::runtime_error("Error casting directive");
// 			}
// 			server.setErrPage(std::stoi(directive->getArguments()[0]), directive->getArguments()[1]);
// 		} else if ((*child)->getName() == "client_max_body_size") {
// 			Directive *directive = dynamic_cast<Directive*>(*child);
// 			if (!directive) {
// 				throw std::runtime_error("Error casting directive");
// 			}
// 			server.setCliMaxBody(std::stoi(directive->getArguments()[0]));
// 		} else if ((*child)->getName() == "location") {
// 			Location loc;
// 			loc.setPath((*child)->getArguments()[0]);
// 			Block *location_block = dynamic_cast<Block*>(*child);
// 			std::vector<ASTNode *> locationChildren = location_block->getChildren();
// 			for (std::vector<ASTNode *>::iterator location_child = locationChildren.begin(); location_child != locationChildren.end(); ++location_child) {
// 				if ((*location_child)->getName() == "root") {
// 					Directive *directive = dynamic_cast<Directive*>(*location_child);
// 					if (!directive) {
// 						throw std::runtime_error("Error casting directive");
// 					}
// 					loc.setRoot(directive->getArguments()[0]);
// 				} else if ((*location_child)->getName() == "index") {
// 					Directive *directive = dynamic_cast<Directive*>(*location_child);
// 					if (!directive) {
// 						throw std::runtime_error("Error casting directive");
// 					}
// 					loc.setIndex(directive->getArguments()[0]);
// 				} else if ((*location_child)->getName() == "methods") {
// 					Directive *directive = dynamic_cast<Directive*>(*location_child);
// 					if (!directive) {
// 						throw std::runtime_error("Error casting directive");
// 					}
// 					unsigned int methods = 0;
// 					std::vector<std::string> directiveArguments = directive->getArguments();
// 					for (std::vector<std::string>::iterator method = directiveArguments.begin(); method != directiveArguments.end(); ++method) {
// 						if (*method == "GET") {
// 							methods |= GET;
// 						} else if (*method == "POST") {
// 							methods |= POST;
// 						} else if (*method == "PUT") {
// 							methods |= PUT;
// 						} else if (*method == "DELETE") {
// 							methods |= DELETE;
// 						}
// 					}
// 					loc.setMethods(methods);
// 				} else if ((*location_child)->getName() == "cgi") {
// 					Directive *directive = dynamic_cast<Directive*>(*location_child);
// 					if (!directive) {
// 						throw std::runtime_error("Error casting directive");
// 					}
// 					loc.setCgi(directive->getArguments()[0], directive->getArguments()[1]);
// 				} else if ((*location_child)->getName() == "autoindex") {
// 					Directive *directive = dynamic_cast<Directive*>(*location_child);
// 					if (!directive) {
// 						throw std::runtime_error("Error casting directive");
// 					}
// 					loc.setDirListing(std::stoi(directive->getArguments()[0]));
// 				}
// 			}
// 			server.setLocation(loc);
// 		}
// 	}
// }

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
				server.setServName(args[0]);
			} else if (name == "root") {
				server.setRoot(args[0]);
			} else if (name == "error_page") {
				if (args.size() == 2) {
					server.setErrPage(std::stoi(args[0]), args[1]);
				} else if (args.size() == 1) {
					server.setErrPage(std::stoi(args[0]), "");
				}
			} else if (name == "client_max_body_size") {
				if (args.size() == 1) {
					server.setCliMaxBody(std::stoi(args[0]));
				}
			}
		} else {
			Block *childBlock = dynamic_cast<Block*>(*child);
			if (childBlock && childBlock->getName() == "location") {
				processLocationBlock(*child, server);
			}
		}

		// if (!directive && (*child)->getName() != "location") {
		// 	throw std::runtime_error("Error casting directive");
		// }

		// std::string name = (*child)->getName();
		// std::vector<std::string> &args = directive->getArguments();

		// if (name == "listen") {
		// 	processListenDirective(args, server);
		// } else if (name == "server_name") {
		// 	server.setServName(args[0]);
		// } else if (name == "root") {
		// 	server.setRoot(args[0]);
		// } else if (name == "error_page") {
		// 	if (args.size() == 2) {
		// 		server.setErrPage(std::stoi(args[0]), args[1]);
		// 	} else if (args.size() == 1) {
		// 		server.setErrPage(std::stoi(args[0]), "");
		// 	}
		// } else if (name == "client_max_body_size") {
		// 	if (args.size() == 1) {
		// 		server.setCliMaxBody(std::stoi(args[0]));
		// 	}
		// } else if (name == "location") {
		// 	processLocationBlock(*child, server);
		// }
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
		} else if (name == "methods") {
			loc.setMethods(parseMethods(args));
		} else if (name == "cgi") {
			try {
				loc.setCgi(args[0], args[1]);
			} catch (std::exception &e) {
				continue;
				// throw std::runtime_error("Insufficient arguments for cgi directive");
			}
		} else if (name == "autoindex") {
			try {
				loc.setDirListing(std::stoi(args[0]));
			} catch (std::exception &e) {
				continue;
				// throw std::runtime_error("Invalid argument for autoindex directive");
			}
		}
	}

	server.setLocation(loc);
}

unsigned int Configuration::parseMethods(std::vector<std::string> &methods) {
	unsigned int methodFlags = 0;
	for (std::vector<std::string>::iterator method = methods.begin(); method != methods.end(); ++method) {
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




std::vector<Server> &Configuration::getServs(void) {
	return (this->_servs);
}
