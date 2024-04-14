#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

void Request::setFinish() {
    _finish = true;
}

void Request::append(char const *bf, size_t n) {
    _request.append(bf, n);
}

std::string &Request::get() {
    return _request;
}
