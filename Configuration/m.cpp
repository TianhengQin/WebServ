
#include "ASTNode.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <stack>




// Trim whitespace from both ends of a string
void parseConfig(std::istream& input) {
    std::stack<std::shared_ptr<Block> > blockStack;
    std::string line;

    while (getline(input, line)) {
        trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line.back() == '{') {
            std::pair<std::string, std::vector<std::string> >   blockNameArgs;
            Block                                               *block;

            blockNameArgs = parseNameAndArguments(line.substr(0, line.size() - 1));
            block = new Block(directiveNameArgs.first, directiveNameArgs.second);
            
            if (!blockStack.empty()) {
                blockStack.top()->addChild(block);
            }
            blockStack.push(block);
        } else if (line == "}") {
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

