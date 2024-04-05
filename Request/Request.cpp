#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

void Request::setFinish() {
    _finish = true;
}

void Request::append(char const *bf) {
    _request.append(bf);
}
