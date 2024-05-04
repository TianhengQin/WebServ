#include "Connection.hpp"

Connection::Connection() {}

Connection::Connection(std::map<int, Server> &svs, int fd) {
    _server.insert(std::make_pair(svs[fd].getName(), svs[fd]));
    fd += 1024;
    while(svs.count(fd)) {
        _server.insert(std::make_pair(svs[fd].getName(), svs[fd]));
        fd += 1024;
    }
}

Connection::~Connection() {}

void Connection::setFd(int fd) {
    this->_fd = fd;
}

int Connection::getFd() {
    return _fd;
}

void Connection::buildResponse() {

    std::ofstream f2("test.jpeg", std::fstream::trunc | std::fstream::binary);

    std::size_t pos = _quest.get().find("\r\n\r\n");
    if (pos != std::string::npos)
        f2 << _quest.get().substr(pos + 4);
    f2.close();
    std::cout << _quest.get().substr(0,500) << std::endl;
    Log::print(INFO, "size :", _quest.get().size());

    std::string line;
    std::string html;
    std::ifstream myfile("_test/website/test.html");
    if (myfile.is_open()) {
        while (std::getline(myfile, line)) {
            html = html + line;
        }
        myfile.close();
    }
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    _sendBf = ss.str();
    _keepAlive = false;
}

void Connection::setResponse(std::string &bf) {
    _sendBf = bf;
}

void Connection::updateTime() {
    _timeStamp = std::time(nullptr);
}

std::time_t Connection::getTimeStamp() {
    return _timeStamp;
}

void Connection::receive(char const *bf, size_t rcvd) {
    _quest.append(bf, rcvd);
    updateTime();
}

int Connection::send() {
    int sent;
    size_t bf_size = _sendBf.size();
    Log::print(DEBUG, "Response buffer ", bf_size);
    if (bf_size <= RS_BF_SIZE) {
        sent = write(_fd, _sendBf.c_str(), bf_size);
        if (sent < 0) {
            close(_fd);
            Log::print(ERROR, "Write error on fd ", _fd);
            throw std::runtime_error("Write Failed");
        }
        Log::print(DEBUG, "Response sent ", sent);
        _sendBf.clear();
        updateTime();
        return 0;
    }
    sent = write(_fd, _sendBf.c_str(), RS_BF_SIZE);
    if (sent < 0) {
        close(_fd);
        Log::print(ERROR, "Write error on fd ", _fd);
        throw std::runtime_error("Write Failed");
    }
    _sendBf.erase(0, RS_BF_SIZE);
    updateTime();
    return 1;
}

bool Connection::session() {
    return _keepAlive;
}

int Connection::cgiState() {
    return _cgiState;
}

void Connection::setCgiState(int s) {
    _cgiState = s;
}

std::string &Connection::getCgiProgram() {
    return _cgiProgram;
}

std::string &Connection::getCgiScript() {
    return _cgiScript;
}

void Connection::buildCgiResponse(std::string const &bd) {
    _sendBf = bd;
}
