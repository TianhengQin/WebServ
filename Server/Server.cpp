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
		this->_socketAddrLen = sv._socketAddrLen;
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

void Server::setup() {

	_listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenFd > FD_SETSIZE) {
		Log::print(WARNING, "Maximum number of fds reached ", _listenFd);
		throw std::out_of_range("Max Fd Num");
	}
	Log::print(INFO, "Listening ", _listenFd);
	int opt = 1;
	if (setsockopt(_listenFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
		Log::print(ERROR, "Set socket address reuse failed ", 0);
		throw std::runtime_error("Set Address Reuse Failed");
	}
	_socketAddr.sin_family = AF_INET;
	_socketAddr.sin_addr.s_addr = _host;
	_socketAddr.sin_port = htons(_port);
	Log::print(INFO, "  Socket ", _host);
	Log::print(INFO, "  port ", _port);
	if (bind(_listenFd, (sockaddr *)&_socketAddr, _socketAddrLen) < 0) {
		Log::print(ERROR, "Socket bind failed on ", _listenFd);
		throw std::runtime_error("Socket Bind Failed");
	}
}

/**
 * Getters
*/

std::string Server::getName(void) {
	return this->_servName;
}

std::string Server::getRoot(void) {
	return this->_root;
}

std::string Server::getIndex(void) {
	return this->_index;
}

unsigned int Server::getHost(void) {
	return this->_host;
}

int Server::getFd(void) {
	return this->_listenFd;
}

unsigned int Server::getPort(void) {
	return this->_port;
}

unsigned int Server::getCliMaxBody(void) {
	return this->_cliMaxBody;
}

/**
 * Setters
*/

void Server::setServName(std::string name) {
	this->_servName = name;
}

void Server::setRoot(std::string root) {
	this->_root = root;
}

void Server::setIndex(std::string index) {
	this->_index = index;
}

void Server::setHost(std::string host) {
	this->_host = inet_addr(host.c_str());
}

void Server::setPort(unsigned int port) {
	this->_port = port;
}

void Server::setCliMaxBody(unsigned int cmb) {
	this->_cliMaxBody = cmb;
}

void Server::setErrPage(int code, std::string path) {
	this->_errorPage[code] = path;
}

void Server::setLocation(Location &loc) {
	this->_locations.push_back(loc);
}
