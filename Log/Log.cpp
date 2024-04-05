#include "Log.hpp"

void Log::print(int level, std::string const &msg, unsigned long i) {

    if (level < logLevel) {
        return;
    }
    if (level == DEBUG) {
        std::cout << "   [DEBUG]   " << msg;
        if (i) std::cout << i;
        std::cout << std::endl;
    } else if (level == INFO) {
        std::cout << "   [INFO]    " << msg;
        if (i) std::cout << i;
        std::cout << std::endl;
    } else if (level == WARNING) {
        std::cerr << "  ![WARNING] " << msg;
        if (i) std::cerr << i;
        std::cerr << std::endl;
    } else if (level == ERROR) {
        std::cerr << "!!![ERROR]!! " << msg;
        if (i) std::cerr << i;
        std::cerr << std::endl;
    }
}
