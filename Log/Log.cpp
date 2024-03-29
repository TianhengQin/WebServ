#include "Log.hpp"

void Log::print(int level, std::string &msg) {

    if (level < logLevel) {
        return;
    }
    if (level == DEBUG) {
        std::cout << "   [DEBUG]   " << msg << std::endl;
    } else if (level == INFO) {
        std::cout << "   [INFO]    " << msg << std::endl;
    } else if (level == WARNING) {
        std::cerr << "  ![WARNING] " << msg << std::endl;
    } else if (level == ERROR) {
        std::cerr << "!!![ERROR]   " << msg << std::endl;
    }
}
