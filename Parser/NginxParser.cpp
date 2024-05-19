
#include "NginxParser.hpp"

NginxParser::NginxParser(void) : _input(std::cin) { }

NginxParser::NginxParser(const NginxParser &other) : _input(other._input)
{
	*this = other;
}

NginxParser &NginxParser::operator=(const NginxParser &other)
{
	if (this != &other) {
		// this->_input = other._input;
		this->_root = other._root;
	}
	return (*this);
}

NginxParser::~NginxParser() { }


NginxParser::NginxParser(std::istream& input) : _input(input) {
	this->parse();
}

Block *NginxParser::getRoot(void) const {
	return this->_root;
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

	this->_root = new Block("root", std::vector<std::string>());
	blockStack.push(this->_root);
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
				this->_root->addChild(blockStack.top());
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
