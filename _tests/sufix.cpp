#include <iostream>
#include <string>


void setMimeType(std::string const &path) {
    std::size_t dot = path.rfind(".");
    if (dot == std::string::npos) {
        std::cout << path << std::endl;
    } else {
        std::cout << path.substr(dot + 1) << std::endl;
    }
}

int main() {
    setMimeType("as.f/aaavi");
    return 0;
}
