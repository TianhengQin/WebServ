#include "Server.hpp"

Server::Server() {
    this->_port = 0;
    this->_host = 0;
    this->_servName = "";
    this->_root = "";
    this->_cliMaxBody = UINT_MAX;
    this->_index = "";
    this->_listenFd = 0;
    this->initErrorPages();
    this->_socketAddrLen = sizeof(_socketAddr);
    memset(&_socketAddr, 0, _socketAddrLen);
}

Server::~Server() {}

Server::Server(const Server &sv) {
    *this = sv;
}

Server &Server::operator=(const Server &sv) {

    if (this != &sv) {
        this->_servName = sv._servName;
        this->_root = sv._root;
        this->_port = sv._port;
        this->_host = sv._host;
        this->_cliMaxBody = sv._cliMaxBody;
        this->_index = sv._index;
        this->_errorPage = sv._errorPage;
        this->_locations = sv._locations;
        this->_listenFd = sv._listenFd;
        this->_socketAddr = sv._socketAddr;
    }
    return (*this);
}

void Server::initErrorPages() {
    _errorPage[301] = "";
    _errorPage[302] = "";
    _errorPage[400] = "";
    _errorPage[401] = "";
    _errorPage[403] = "";
    _errorPage[404] = "";
    _errorPage[405] = "";
    _errorPage[406] = "";
    _errorPage[500] = "";
    _errorPage[501] = "";
    _errorPage[502] = "";
    _errorPage[503] = "";
    _errorPage[505] = "";
}

int Server::getFd() {
    return _listenFd;
}

void Server::setup() {

    _listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenFd > FD_SETSIZE) {
        Log::print(WARNING, "Maximum number of fds reached");
        throw std::out_of_range("Max Fd Num");
    }
    int option_value = 1;
    setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_addr.s_addr = _host;
    _socketAddr.sin_port = htons(_port);
    bind(_listenFd, (struct sockaddr *)&_socketAddr, _socketAddrLen);
}
