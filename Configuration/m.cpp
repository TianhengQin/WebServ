
#include "Configuration.hpp"
// #include "ASTNode.hpp"
// #include "NginxParser.hpp"


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

    // parse_configuration_file(input);
    // Block *root = parse_configuration_file(argv[1]);

    Configuration config(argv[1]);
    return 0;
}



