#include "Connection.hpp"

Connection::Connection() {}

Connection::Connection(std::map<int, Server> &svs, int fd) {
    _server.insert(std::make_pair(fd, svs[fd]));
    _servChoice = fd;
    _locationChoice = 0;
    fd += 1024;
    while(svs.count(fd)) {
        _server.insert(std::make_pair(fd, svs[fd]));
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

    // PUT test
    std::ofstream f2("test.jpeg", std::fstream::trunc | std::fstream::binary);

    std::size_t pos = _quest.get().find("\r\n\r\n");
    if (pos != std::string::npos) {
        std::cerr << "no body" << std::endl;
        f2 << _quest.get().substr(pos + 4);
    }
    f2.close();
    std::cout << _quest.get() << std::endl;
    Log::print(INFO, "size :", _quest.get().size());

    // std::string line;
    // std::string html;
    // // std::ifstream myfile("_test/website/test.html");
    // std::ifstream myfile("./index.html");
    // if (myfile.is_open()) {
    //     while (std::getline(myfile, line)) {
    //         html = html + line;
    //     }
    //     myfile.close();
    // }


    //test timo
    // _quest.init("");
    _quest.parse();
    _sponse.init(_quest, false);
    // std::cout << _sponse.generate() << std::endl;
    


    std::ostringstream ss(_sponse.generate());
    // ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n" << html;
    _sendBf = ss.str();
    _keepAlive = false;
    // _quest.clear();
    // CGI test
    // std::cout << _quest.get().substr(0,500) << std::endl;
    // // _cgiProgram = "/usr/local/bin/python3";
    // // _cgiScript = "_test/website/cgi_test.py";
    // _cgiProgram = "/bin/bash";
    // _cgiScript = "_test/website/cgi_test.sh";
    // _cgiSendBf = "[cgi request body]";
    // _cgiState = CGI_ON;
    // _keepAlive = false;
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

std::string &Connection::getCgiSendBf() {
    return _cgiSendBf;
}

void Connection::buildCgiResponse(std::string const &bd) {
    _sendBf = bd;
    std::cout << _sendBf << std::endl;
    Log::print(DEBUG, "cgi received ", _sendBf.size());
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Length: " << _sendBf.size() << "\n" << _sendBf;
    _sendBf = ss.str();
}

std::string Connection::getCookie() {
    return _quest.get_cookie();
}

int Connection::saveFile(std::string const &file) {

    std::ofstream f2(file, std::fstream::trunc | std::fstream::binary);
    if (f2.fail()) {
        return 1;
    }
    std::size_t pos = _quest.get().find("\r\n\r\n");
    if (pos == std::string::npos) {
        f2.close();
        return 1;
    }
    f2 << _quest.get().substr(pos + 4);
    f2.close();
    return 0;
}

std::string Connection::getUri() {
    return std::string("http://")+_quest.get_host()+":"+_quest.get_port()+_quest.get_dir();
}

std::string Connection::getHost() {
    return _quest.get_host();
}

std::string Connection::getMethod() {
    int method = _quest.get_method();
    if (method == GET) {
        return "GET";
    } else if (method == POST) {
        return "POST";
    } else if (method == DELETE) {
        return "DELETE";
    }
    return "PUT";
}

std::string Connection::getQuery() {
    // return _quest.get_query();
    return "";
}

std::size_t Connection::getBodyLen() {
    return _cgiSendBf.size();
}

std::string Connection::getMimeType() {
    return "";
}

std::string Connection::getServName() {
    return _server[_servChoice].getServerName();
}

std::string Connection::getPort() {
    return _quest.get_port();
}

void Connection::chooseServer() {
    int i = _servChoice;
    while (_server.count(i)) {
        if (_server[i].getServerName() == _quest.get_host()) {
            _servChoice = i;
            return;
        }
        i += 1024;
    }
}
