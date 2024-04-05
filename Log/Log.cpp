#include "Log.hpp"

void Log::print(int level, std::string const &msg, unsigned long i) {

    if (level < logLevel) {
        return;
    }
    if (level == DEBUG) {
        std::cout << "   [DEBUG]   " << msg << i << std::endl;
    } else if (level == INFO) {
        std::cout << "   [INFO]    " << msg << i << std::endl;
    } else if (level == WARNING) {
        std::cerr << "  ![WARNING] " << msg << i << std::endl;
    } else if (level == ERROR) {
        std::cerr << "!!![ERROR]!! " << msg << i << std::endl;
    }
}
