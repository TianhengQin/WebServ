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
    updateTime();
}

int Connection::send() {
    size_t sent;
    size_t bf_size = _sendBf.size();
    if (bf_size <= RS_BF_SIZE) {
        sent = write(_fd, _sendBf.c_str(), RS_BF_SIZE);
        if (sent < 0) {
            throw std::runtime_error("Write Failed");
        }
        _sendBf.clear();
        return 0;
    }
    sent = write(_fd, _sendBf.c_str(), RS_BF_SIZE);
    if (sent < 0) {
        throw std::runtime_error("Write Failed");
    }
    _sendBf.erase(0, RS_BF_SIZE);
    return 1;
}
