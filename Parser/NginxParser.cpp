
#include "NginxParser.hpp"
#include "Location.hpp"


NginxParser::NginxParser(void) : _input(std::cin) {
	this->ast_root = new Block("root", std::vector<std::string>());
}

NginxParser::NginxParser(const NginxParser &other) : _input(other._input) {
	*this = other;
}

NginxParser &NginxParser::operator=(const NginxParser &other) {
	if (this != &other) {
		this->ast_root = other.ast_root;
	}
	return (*this);
}

NginxParser::~NginxParser() {
	delete this->ast_root;
}



NginxParser::NginxParser(std::istream& input) : _input(input) {
	this->ast_root = new Block("root", std::vector<std::string>());
	this->parse();
}


void	NginxParser::parse(void) {
	std::pair<std::string, std::vector<std::string> >	name_args;
	Block					*block;
	Directive				*directive;
	std::stack< Block * >	blockStack;
	std::string				line;

	blockStack.push(this->ast_root);
	while (getline(_input, line)) {
		trim(line);
		if (line.empty() || line[0] == '#') continue;

		if (line[line.size() - 1] == '{') {
			name_args = parseNameAndArguments(line.substr(0, line.size() - 1));
			block = new Block(name_args.first, name_args.second);
			if (!blockStack.empty()) {
				blockStack.top()->addChild(block);
			}
			blockStack.push(block);
		} else if (line == "}") {
			if (blockStack.empty()) {
				std::cerr << "Unmatched '}'" << std::endl;
				continue;
			} else if (blockStack.size() == 1) {
				this->ast_root->addChild(blockStack.top());
			}
			blockStack.pop();
		} else {
			name_args = parseNameAndArguments(line.substr(0, line.size() - 1));
			directive = new Directive(name_args.first, name_args.second);
			if (!blockStack.empty()) {
				blockStack.top()->addChild(directive);
			}
		}
	}
}




std::pair<std::string, std::vector<std::string> > NginxParser::parseNameAndArguments(const std::string& line) {
	std::string					name;
	std::vector<std::string>	arguments;
	std::string::size_type		pos;
	std::string::size_type		nextPos;

	pos = line.find_first_of(" \t");
	if (pos == std::string::npos) {
		name = line;
	} else {
		name = line.substr(0, pos);
		while (pos != std::string::npos) {
			pos = line.find_first_not_of(" \t", pos);
			if (pos == std::string::npos) break;
			nextPos = line.find_first_of(" \t", pos);
			arguments.push_back(line.substr(pos, nextPos - pos));
			pos = nextPos;
		}
	}
	return std::make_pair(name, arguments);
}



bool	NginxParser::is_absolute_path(std::string &path) {
	if (path.empty())
		return false;
	return (path[0] == '/');
}

unsigned int	NginxParser::parse_size(std::string &sizeStr) {
	size_t numEnd = 0;
	while (numEnd < sizeStr.size() && std::isdigit(sizeStr[numEnd])) {
		numEnd++;
	}

	unsigned int size = 1;
	try {
		size = std::atoi(sizeStr.substr(0, numEnd).c_str());
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

	return (size);
}

unsigned int	NginxParser::parse_method(std::vector<std::string> &methods) {
	unsigned int allowed_methods = 0;
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it) {
		std::transform(it->begin(), it->end(), it->begin(), ::toupper);

		if (*it == "GET") {
			allowed_methods |= GET;
		} else if (*it == "POST") {
			allowed_methods |= POST;
		} else if (*it == "DELETE") {
			allowed_methods |= DELETE;
		} else if (*it == "PUT") {
			allowed_methods |= PUT;
		} else if (*it == "HEAD") {
			allowed_methods |= HEAD;
		}
	}
	return (allowed_methods);
}

void	NginxParser::ltrim(std::string &s) {
	std::string::iterator it = std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
	s.erase(s.begin(), it);
}

void	NginxParser::rtrim(std::string &s) {
	std::string::reverse_iterator it = std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)));
	s.erase(it.base(), s.end());
}

void	NginxParser::trim(std::string &s) {
	this->ltrim(s);
	std::string::size_type pos = s.find('#');
	if (pos != std::string::npos) {
		s.erase(pos);
	}
	this->rtrim(s);
}


/**
 * Getters
 */

Block *NginxParser::getRoot(void) const {
	return this->ast_root;
}

Block *NginxParser::getHttpBlock(void) const {
	std::vector<ASTNode *>				children;
	std::vector<ASTNode *>::iterator	child;
	
	children = this->ast_root->getChildren();
	for (child = children.begin(); child != children.end(); ++child) {
		Block *block = dynamic_cast<Block*>(*child);
		if (block && block->getName() == "http") {
			return block;
		}
	}
	throw std::runtime_error("Error: no http block found");
	return NULL;
}

void	NginxParser::parse_configuration_file(void) {
	std::string line;
	std::string name;
	std::string value;
	std::string::size_type pos;

	while (getline(_input, line)) {
		trim(line);
		if (line.empty() || line[0] == '#') continue;
		pos = line.find_first_of(" \t");
		if (pos == std::string::npos) {
			name = line;
			value = "";
		} else {
			name = line.substr(0, pos);
			value = line.substr(pos + 1);
		}
		trim(name);
		trim(value);
		if (name == "root") {
			this->ast_root->addChild(new Directive(name, std::vector<std::string>(1, value)));
		}
	}
}

/**
 * Overload the << operator to print the AST
 */


std::ostream &operator<<(std::ostream &out, NginxParser &parser) {
	ASTNode *node = parser.getRoot();
	node->print();
	return out;
}
