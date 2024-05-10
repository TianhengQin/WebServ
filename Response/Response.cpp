#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::init() {

}

void Response::setMimeType(std::string const &path) {
    std::size_t dot = path.rfind(".");
    if (dot == std::string::npos) {
        _mimeType = _mmTy.getType("bin");
    } else {
        _mimeType = _mmTy.getType(path.substr(dot + 1));
    }
}

int Response::setBody(std::string const &file) {
    std::string line;
    std::ifstream myfile(file);
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            _body = _body + line;
        }
        myfile.close();
        return 0;
    }
    return 1;
}
