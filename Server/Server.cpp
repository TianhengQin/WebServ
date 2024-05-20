#include "Server.hpp"

Server::Server(void) {
	this->_server_name = "";
	this->_root = "";
	this->_index = "";
	this->_socket_address_length = sizeof(_socket_address);
	memset(&_socket_address, 0, _socket_address_length);
	this->_host = 0;
	this->_port = 0;
	this->_client_max_body_size = UINT_MAX;
	this->_default = false;
	this->_listenFd = 0;
	this->_autoindex = false;
	initDefaultErrorPages();

}

Server::~Server() {}

Server::Server(const Server &sv) {
	*this = sv;
}

Server &Server::operator=(const Server &sv) {

	if (this != &sv) {
		this->_server_name = sv._server_name;
		this->_root = sv._root;
		this->_port = sv._port;
		this->_host = sv._host;
		this->_client_max_body_size = sv._client_max_body_size;
		this->_index = sv._index;
		this->_error_page = sv._error_page;
		this->_locations = sv._locations;
		this->_listenFd = sv._listenFd;
		this->_socket_address = sv._socket_address;
		this->_socket_address_length = sv._socket_address_length;
	}
	return (*this);
}

void Server::initDefaultErrorPages(void) {
	this->_error_page[301] = "HTML/default_error_pages/301.html";
	this->_error_page[302] = "HTML/default_error_pages/302.html";
	this->_error_page[400] = "HTML/default_error_pages/400.html";
	this->_error_page[401] = "HTML/default_error_pages/401.html";
	this->_error_page[403] = "HTML/default_error_pages/403.html";
	this->_error_page[404] = "HTML/default_error_pages/404.html";
	this->_error_page[405] = "HTML/default_error_pages/405.html";
	this->_error_page[406] = "HTML/default_error_pages/406.html";
	this->_error_page[500] = "HTML/default_error_pages/500.html";
	this->_error_page[501] = "HTML/default_error_pages/501.html";
	this->_error_page[502] = "HTML/default_error_pages/502.html";
	this->_error_page[503] = "HTML/default_error_pages/503.html";
	this->_error_page[505] = "HTML/default_error_pages/505.html";
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
	_socket_address.sin_family = AF_INET;
	_socket_address.sin_addr.s_addr = _host;
	_socket_address.sin_port = htons(_port);
	Log::print(INFO, "  Socket ", _host);
	Log::print(INFO, "  port ", _port);
	if (bind(_listenFd, (sockaddr *)&_socket_address, _socket_address_length) < 0) {
		Log::print(ERROR, "Socket bind failed on ", _listenFd);
		throw std::runtime_error("Socket Bind Failed");
	}
}

/**
 * Getters
*/

std::string Server::getServerName(void) {
	return this->_server_name;
}

std::string Server::getRoot(void) {
	return this->_root;
}

std::string Server::getIndex(void) {
	return this->_index;
}

std::string Server::getHostStr(void) {
	return this->_listen;
}

unsigned int Server::getHost(void) {
	return this->_host;
}

unsigned int Server::getPort(void) {
	return this->_port;
}

unsigned int Server::getClientMaxBodySize(void) {
	return this->_client_max_body_size;
}

bool Server::getDefault(void) {
	return this->_default;
}

int Server::getFd(void) {
	return this->_listenFd;
}

bool Server::getAutoindex(void) {
	return this->_autoindex;
}


std::vector<Location> Server::getLocations(void) {
	return this->_locations;
}

std::map<int, std::string> Server::getErrorPages(void) {
	return this->_error_page;
}


std::vector<std::string> Server::get_allServerNames(void) {
	return this->_all_server_names;
}

std::vector<std::string> Server::get_allIndexes(void) {
	return this->_all_index;
}

/**
 * Setters
*/

void	Server::setServerName(std::string name) {
	if (_server_name.empty())
		_server_name = name;
	this->_all_server_names.push_back(name);
}

void	Server::setRoot(std::string root) {
	this->_root = root;
}

void	Server::setIndex(std::string index) {
	if (_index.empty())
		_index = index;
	this->_all_index.push_back(index);
}

void	Server::setHost(std::string host) {
	this->_listen = host;
	this->_host = inet_addr(host.c_str());
}

void	Server::setPort(unsigned int port) {
	this->_port = port;
}

void	Server::setClientMaxBodySize(unsigned int cmb) {
	this->_client_max_body_size = cmb;
}

void	Server::setDefault(bool def) {
	this->_default = def;
}

void	Server::setListenFd(int fd) {
	this->_listenFd = fd;
}

void	Server::setAutoindex(bool ai) {
	this->_autoindex = ai;
}

void	Server::setErrorPage(int code, std::string path) {
	this->_error_page[code] = path;
}

void	Server::setLocation(Location &loc) {
	this->_locations.push_back(loc);
}

std::ostream &operator<<(std::ostream &os, Server &sv) {
	os << "  Server: " << sv.getServerName() << std::endl;
	os << "    Host: " << sv.getHostStr() << std::endl;
	os << "    Port: " << sv.getPort() << std::endl;
	os << "    Root: " << sv.getRoot() << std::endl;
	os << "    Index: " << sv.getIndex() << std::endl;
	os << "    Client Max Body: " << sv.getClientMaxBodySize() << std::endl;
	os << "    Default: " << sv.getDefault() << std::endl;
	os << "    Listen Fd: " << sv.getFd() << std::endl;
	std::vector<Location> locs = sv.getLocations();
	for (std::vector<Location>::iterator loc = locs.begin(); loc != locs.end(); ++loc) {
		os << *loc << std::endl;
	}
	return os;
}
