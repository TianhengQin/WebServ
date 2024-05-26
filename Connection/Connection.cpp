#include "Connection.hpp"

Connection::Connection() {}

Connection::Connection(std::map<int, Server> &svs, int fd) {
    _timeStamp = std::time(NULL);
    _server.insert(std::make_pair(fd, svs[fd]));
    _servChoice = fd;
    _locationChoice = 0;
    fd += 1024;
    while(svs.count(fd)) {
        _server.insert(std::make_pair(fd, svs[fd]));
        fd += 1024;
    }
    _keepAlive = false;
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
    // std::ofstream f2("test.jpeg", std::fstream::trunc | std::fstream::binary);

    // std::size_t pos = _quest.get().find("\r\n\r\n");
    // if (pos != std::string::npos) {
    //     f2 << _quest.get().substr(pos + 4);
    // }
    // f2.close();
    // Log::print(INFO, "size :", _quest.get().size());


    _quest.parse();

    chooseServer();
    std::vector<Location>   locations = _server[_servChoice].getLocations();
    Location                match;
    size_t                  match_length = 0;
    bool                    match_found = false;

    for (size_t i = 0; i < locations.size(); ++i) {
        if (_quest.get_dir().size() >= locations[i].getPath().size()) {
            if (locations[i].getPath() == _quest.get_dir().substr(0, locations[i].getPath().size())) {
                if (locations[i].getPath().size() > match_length) {
                    match = locations[i];
                    match_length = locations[i].getPath().size();
                    match_found = true;
                }
            }
        }
    }
    if (match_found) {
        _sponse.init(*this, _quest, _server[_servChoice], match);

    } else {
        _sponse.set_code(404);
    }

    

    if (_cgiState == CGI_ON) {
        return ;
    }

    std::ostringstream ss(_sponse.generate());
    
    _sendBf = ss.str();
}

void Connection::setResponse(std::string &bf) {
    _sendBf = bf;
}

void Connection::updateTime() {
    _timeStamp = std::time(NULL);
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
    if (bf_size <= RS_BF_SIZE) {
        sent = write(_fd, _sendBf.c_str(), bf_size);
        if (sent < 0) {
            close(_fd);
            Log::print(ERROR, "Write error on fd ", _fd);
            throw std::runtime_error("Write Failed");
        }
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
    if (s == CGI_TIMEOUT) {
        _sponse.set_code(504);
        _sendBf = _sponse.generate();

    } else if (s == CGI_FAILED) {
        _sponse.set_code(500);
        _sendBf = _sponse.generate();

    }
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
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Length: " << _sendBf.size() << "\n" << _sendBf;
    _sendBf = ss.str();
}

std::string Connection::getCookie() {
    return _quest.get_cookie();
}

int Connection::saveFile(std::string const &file) {

    std::ofstream f2(file.c_str(), std::fstream::trunc | std::fstream::binary);
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
    return _quest.get_query();
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

void    Connection::setCgiProgram(std::string pathToCgi) {
    _cgiProgram = pathToCgi;
}

void    Connection::setCgiScript(std::string realPath) {
    _cgiScript = realPath;
}

void    Connection::setCgiSendBf(std::string bf) {
    _cgiSendBf = bf;
}
