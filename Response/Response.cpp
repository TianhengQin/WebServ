#include "Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::init() {

}

void Response::setMimeType(std::string const &path) {
    std::size_t dot = path.rfind(".");
    if (dot == std::string::npos) {
        _mimeType = mm_ty.getType("bin");
    } else {
        _mimeType = mm_ty.getType(path.substr(dot + 1));
    }
}
