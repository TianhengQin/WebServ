
#include "NginxParser.hpp"

NginxParser::NginxParser(void) : _input(std::cin) {
	this->ast_root = new Block("root", std::vector<std::string>());
}

// NginxParser::NginxParser(void) {
// 	std::cerr << "Error: no filename provided" << std::endl;
// }

NginxParser::NginxParser(const NginxParser &other) : _input(other._input)
{
	*this = other;
}

NginxParser &NginxParser::operator=(const NginxParser &other)
{
	if (this != &other) {
		// TODO: copy
	}
	return (*this);
}


// NginxParser::NginxParser(std::string filename) : _filename(filename), _input(std::ifstream(filename.c_str())) {
// 	this->ast_root = new Block("root", std::vector<std::string>());

// 	// this->_input = std::ifstream(filename.c_str());
// 	// if (!this->_input.is_open()) {
// 	// 	throw std::runtime_error("Error: could not open file " + filename);
// 	// }
// 	// this->parse(this->_file);

// 	// std::ifstream file(this->_filename.c_str());
// 	// if (!file.is_open()) {
// 	// 	throw std::runtime_error("Error opening file: " + this->_filename);
// 	// }
// 	// _input = file;
// 	// this->parse();

// 	// if (!file.is_open()) {
// 	// 	std::cerr << "Error: could not open file " << filename << std::endl;
// 	// 	exit(1);
// 	// }
// 	// this->_input = file;
// 	// this->parse();
// }

NginxParser::NginxParser(std::istream& input) : _input(input) {
	this->ast_root = new Block("root", std::vector<std::string>());
	this->parse();
}

NginxParser::~NginxParser() { }


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
	this->rtrim(s);
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

		if (line.back() == '{') {
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

std::ostream &operator<<(std::ostream &out, NginxParser &parser) {
	ASTNode *node = parser.getRoot();
	node->print();
	// out << *(parser.getRoot());
	return out;
}
