#include "Server.hpp"

Server::Server(void) {
	this->_port = 0;
	this->_host = 0;
	this->_servName = "";
	this->_root = "";
	this->_cliMaxBody = UINT_MAX;
	this->_index = "";
	this->_listenFd = 0;
	initDefaultErrorPages();
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
		this->_error_page = sv._error_page;
		this->_locations = sv._locations;
		this->_listenFd = sv._listenFd;
		this->_socketAddr = sv._socketAddr;
		this->_socketAddrLen = sv._socketAddrLen;
	}
	return (*this);
}

void Server::initDefaultErrorPages(void) {
	this->_error_page[301] = "config-files/default_error_pages/301.html";
	this->_error_page[302] = "config-files/default_error_pages/302.html";
	this->_error_page[400] = "config-files/default_error_pages/400.html";
	this->_error_page[401] = "config-files/default_error_pages/401.html";
	this->_error_page[403] = "config-files/default_error_pages/403.html";
	this->_error_page[404] = "config-files/default_error_pages/404.html";
	this->_error_page[405] = "config-files/default_error_pages/405.html";
	this->_error_page[406] = "config-files/default_error_pages/406.html";
	this->_error_page[500] = "config-files/default_error_pages/500.html";
	this->_error_page[501] = "config-files/default_error_pages/501.html";
	this->_error_page[502] = "config-files/default_error_pages/502.html";
	this->_error_page[503] = "config-files/default_error_pages/503.html";
	this->_error_page[505] = "config-files/default_error_pages/505.html";
}

void Server::setup(void) {

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

std::string &Server::getName(void) {
	return this->_servName;
}

std::string Server::getRoot(void) {
	return this->_root;
}

std::string Server::getIndex(void) {
	return this->_index;
}

std::string Server::getHostStr(void) {
	return this->_hostStr;
}

unsigned int Server::getHost(void) {
	return this->_host;
}

unsigned int Server::getPort(void) {
	return this->_port;
}

unsigned int Server::getCliMaxBody(void) {
	return this->_cliMaxBody;
}

bool Server::getDefault(void) {
	return this->_default;
}

int Server::getFd(void) {
	return this->_listenFd;
}


std::vector<Location> &Server::getLocations(void) {
	return this->_locations;
}

/**
 * Setters
*/

void	Server::setServName(std::string name) {
	this->_servName = name;
}

void	Server::setRoot(std::string root) {
	this->_root = root;
}

void	Server::setIndex(std::string index) {
	this->_index = index;
}

void	Server::setHost(std::string host) {
	this->_hostStr = host;
	this->_host = inet_addr(host.c_str());
}

void	Server::setPort(unsigned int port) {
	this->_port = port;
}

void	Server::setCliMaxBody(unsigned int cmb) {
	this->_cliMaxBody = cmb;
}

void	Server::setDefault(bool def) {
	this->_default = def;
}

void	Server::setListenFd(int fd) {
	this->_listenFd = fd;
}

void	Server::setErrPage(int code, std::string path) {
	this->_error_page[code] = path;
}

void	Server::setLocation(Location &loc) {
	this->_locations.push_back(loc);
}

std::ostream &operator<<(std::ostream &os, Server &sv) {
	os << "Server: " << sv.getName() << std::endl;
	os << "  Host: " << sv.getHostStr() << std::endl;
	os << "  Port: " << sv.getPort() << std::endl;
	os << "  Root: " << sv.getRoot() << std::endl;
	os << "  Index: " << sv.getIndex() << std::endl;
	os << "  Client Max Body: " << sv.getCliMaxBody() << std::endl;
	os << "  Default: " << sv.getDefault() << std::endl;
	os << "  Listen Fd: " << sv.getFd() << std::endl;
	os << "  Locations: " << std::endl;
	std::vector<Location> locs = sv.getLocations();
	for (std::vector<Location>::iterator loc = locs.begin(); loc != locs.end(); ++loc) {
		os << "    " << *loc << std::endl;
	}
	return os;
}
