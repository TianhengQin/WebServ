
#include "Head.hpp"

int main(int argc, char **argv) {
    try {
        if (argc != 2) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <config_file>");//throw invalid_argument
        }
        Configuration config(argv[1]);
        // std::cout << "Configuration file: " << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;//Log::print
        return 1;
    }

    return 0;
}
