#include "Connection.hpp"

Connection::Connection(Server const &sv) {

}

Connection::~Connection() {}

void Connection::setFd(int fd) {
    this->_fd = fd;
}

void Connection::buildResponse() {

}

void Connection::updateTime() {
    _timeStamp = std::time(nullptr);
}


void Connection::receive(std::string const &bf) {
    _quest.append(bf);
}
