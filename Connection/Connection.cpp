#include "Connection.hpp"

Connection::Connection(std::map<int, Server> &svs, int fd) {

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

std::time_t Connection::getTimeStamp() {
    return _timeStamp;
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
            Log::print(ERROR, "Write error on fd ", _fd);
            throw std::runtime_error("Write Failed");
        }
        _sendBf.clear();
        updateTime();
        return 0;
    }
    sent = write(_fd, _sendBf.c_str(), RS_BF_SIZE);
    if (sent < 0) {
        Log::print(ERROR, "Write error on fd ", _fd);
        throw std::runtime_error("Write Failed");
    }
    _sendBf.erase(0, RS_BF_SIZE);
    updateTime();
    return 1;
}
