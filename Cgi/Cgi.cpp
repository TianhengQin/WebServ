#include "Cgi.hpp"

Cgi::Cgi() {}

Cgi::Cgi(Connection &conn) {
    _connectFd = conn.getFd();
    _pid = 0;
}

Cgi::~Cgi() {
    close(_pipeIn[0]);
    close(_pipeIn[1]);
    close(_pipeOut[0]);
    close(_pipeOut[1]);
    kill(_pid, SIGKILL);
}

int Cgi::getConnectFd() {
    return _connectFd;
}

int Cgi::getPipeOutFd() {
    return _pipeOut[0];
}

int Cgi::getPipeInFd() {
    return _pipeIn[1];
}

int Cgi::send() {

    int sent;
    size_t bf_size = _sendBf.size();
    Log::print(DEBUG, "Cgi send buffer ", bf_size);
    if (bf_size <= RS_BF_SIZE) {
        sent = write(_pipeIn[1], _sendBf.c_str(), bf_size);
        if (sent < 0) {
            Log::print(ERROR, "Write error on pipe ", _pipeIn[1]);
            return -1;
        }
        Log::print(DEBUG, "Cgi sent ", sent);
        _sendBf.clear();
        return 0;
    }
    sent = write(_pipeIn[1], _sendBf.c_str(), RS_BF_SIZE);
    if (sent < 0) {
        Log::print(ERROR, "Write error on pipe ", _pipeIn[1]);
        return -1;
    }
    _sendBf.erase(0, RS_BF_SIZE);
    return 1;
}

int Cgi::receive() {

    char bf[RS_BF_SIZE + 1];
    int received = read(_pipeOut[0], bf, RS_BF_SIZE);
    if (received < 0) {
        Log::print(ERROR, "Read error on pip ", _pipeOut[0]);
        return -1;
    } else if (received < RS_BF_SIZE) {
        _recvBf.append(bf, received);
        Log::print(DEBUG, "Received cgi finish ", _pipeOut[0]);
        return 0;
    } else {
        _recvBf.append(bf, received);
        return 1;
    }
}

std::string &Cgi::response() {
    return _recvBf;
}
