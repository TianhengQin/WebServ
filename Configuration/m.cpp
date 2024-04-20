
#include "ASTNode.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <stack>
#include <algorithm>
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}


std::pair<std::string, std::vector<std::string> > parseNameAndArguments(const std::string& line) {
    std::pair<std::string, std::vector<std::string> > result;
    std::string::size_type                           pos;
    std::string                                     name;
    std::vector<std::string>                        arguments;

    pos = line.find_first_of(" \t");
    if (pos == std::string::npos) {
        name = line;
    } else {
        name = line.substr(0, pos);
        while (pos != std::string::npos) {
            std::string::size_type   nextPos;

            pos = line.find_first_not_of(" \t", pos);
            if (pos == std::string::npos) break;

            nextPos = line.find_first_of(" \t", pos);
            arguments.push_back(line.substr(pos, nextPos - pos));
            pos = nextPos;
        }
    }

    result.first = name;
    result.second = arguments;
    return result;
}

// Trim whitespace from both ends of a string
void parseConfig(std::istream& input) {
    Block *root = new Block("root", {});
    std::stack< Block * > blockStack;
    std::string line;

    while (getline(input, line)) {
        trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.back() == '{') {
            std::pair<std::string, std::vector<std::string> >   blockNameArgs;
            Block                                               *block;

            blockNameArgs = parseNameAndArguments(line.substr(0, line.size() - 1));
            block = new Block(blockNameArgs.first, blockNameArgs.second);
            
            if (!blockStack.empty()) {
                blockStack.top()->addChild(block);
            }
            blockStack.push(block);
        } else if (line == "}") {
            // Debugging
            if (blockStack.empty()) {
                std::cerr << "Unmatched '}'" << std::endl;
                continue;
            } else if (blockStack.size() == 1) {
                root->addChild(blockStack.top());
            }
            blockStack.pop();
        } else {
            std::pair<std::string, std::vector<std::string> >   directiveNameArgs;
            Directive                                           *directive;

            directiveNameArgs = parseNameAndArguments(line.substr(0, line.size() - 1));
            directive = new Directive(directiveNameArgs.first, directiveNameArgs.second);
            
            if (!blockStack.empty()) {
                blockStack.top()->addChild(directive);
            }
        }
    }

    root->print();
}


int main(int argc, char** argv) {
   
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config file>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    parseConfig(input);
    return 0;
}



