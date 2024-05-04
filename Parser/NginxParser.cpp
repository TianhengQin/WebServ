
#include "NginxParser.hpp"

static std::pair<std::string, std::vector<std::string> > parseNameAndArguments(const std::string& line) {
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


void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}


Block * parseConfig(std::istream& input) {
	std::pair<std::string, std::vector<std::string> >	name_args;
	Block					*root;
	Block					*block;
	Directive				*directive;
	std::stack< Block * >	blockStack;
	std::string				line;

	root = new Block("root", {});
	blockStack.push(root);
	while (getline(input, line)) {
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
				root->addChild(blockStack.top());
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

	root->print();
	return root;
}
