#include "Server.hpp"

Server::Server(void) {
	_listen = "";
	_host = 0;
	_port = 0;
	_server_name = "";
	_root = "";
	_index = "";
	_allow_methods = GET | POST | DELETE | HEAD;
	initDefaultErrorPages();
	_client_max_body_size = 1024 * 1024;
	_autoindex = false;
	_listenFd = 0;
	_socket_address_length = sizeof(_socket_address);
	memset(&_socket_address, 0, _socket_address_length);
}

Server::~Server() {}

Server::Server(const Server &sv) {
	*this = sv;
}

Server &Server::operator=(const Server &sv) {

	if (this != &sv) {
		_listen = sv._listen;
		_host = sv._host;
		_port = sv._port;
		_server_name = sv._server_name;
		_root = sv._root;
		_index = sv._index;
		_allow_methods = sv._allow_methods;
		_error_page = sv._error_page;
		_client_max_body_size = sv._client_max_body_size;
		_autoindex = sv._autoindex;
		_cgi = sv._cgi;
		_locations = sv._locations;
		_socket_address = sv._socket_address;
		_socket_address_length = sv._socket_address_length;
		_listenFd = sv._listenFd;
		_all_server_names = sv._all_server_names;
		_all_index = sv._all_index;
	}
	return (*this);
}

void Server::initDefaultErrorPages(void) {
	this->_error_page[301] = "/HTML/default_error_pages/301.html";
	this->_error_page[302] = "/HTML/default_error_pages/302.html";
	this->_error_page[400] = "/HTML/default_error_pages/400.html";
	this->_error_page[401] = "/HTML/default_error_pages/401.html";
	this->_error_page[403] = "/HTML/default_error_pages/403.html";
	this->_error_page[404] = "/HTML/default_error_pages/404.html";
	this->_error_page[405] = "/HTML/default_error_pages/405.html";
	this->_error_page[406] = "/HTML/default_error_pages/406.html";
	this->_error_page[409] = "/HTML/default_error_pages/409.html";
	this->_error_page[413] = "/HTML/default_error_pages/413.html";
	this->_error_page[500] = "/HTML/default_error_pages/500.html";
	this->_error_page[501] = "/HTML/default_error_pages/501.html";
	this->_error_page[502] = "/HTML/default_error_pages/502.html";
	this->_error_page[503] = "/HTML/default_error_pages/503.html";
	this->_error_page[504] = "/HTML/default_error_pages/504.html";
	this->_error_page[505] = "/HTML/default_error_pages/505.html";
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

std::string	Server::getHostStr(void) { return inet_ntoa(_socket_address.sin_addr); }

unsigned int	Server::getHost(void) { return _host; }

unsigned int	Server::getPort(void) { return _port; }

std::string	Server::getServerName(void) { return _server_name; }

std::string	Server::getRoot(void) { return _root; }

std::string	Server::getIndex(void) { return _index; }

unsigned int	Server::getAllowedMethods(void) { return _allow_methods; }

std::map<int, std::string>	Server::getErrorPages(void) { return _error_page; }

unsigned int	Server::getClientMaxBodySize(void) { return _client_max_body_size; }

bool	Server::getAutoindex(void) { return _autoindex; }

std::map<std::string, std::string>	Server::getCgi(void) { return _cgi; }

int	Server::getFd(void) { return _listenFd; }

std::vector<Location>	Server::getLocations(void) { return _locations; }

std::vector<std::string>	Server::get_allServerNames(void) { return _all_server_names; }

std::vector<std::string>	Server::get_allIndexes(void) { return _all_index; }

/**
 * Setters
*/

void	Server::addServerName(std::string name) {
	if (_server_name.empty())
		_server_name = name;
	this->_all_server_names.push_back(name);
}

void	Server::setRoot(std::string root) {
	this->_root = root;
}

void	Server::addIndex(std::string index) {
	if (_index.empty())
		_index = index;
	this->_all_index.push_back(index);
}

void	Server::setAllowedMethods(unsigned int methods) {
	this->_allow_methods = methods;
}

void	Server::setHost(std::string host) {
	this->_listen = host;
	this->_host = inet_addr(host.c_str());
}

void	Server::setPort(unsigned int port) {
	this->_port = port;
}

void	Server::setClientMaxBodySize(unsigned int client_max_body_size) {
	this->_client_max_body_size = client_max_body_size;
}

void	Server::setListenFd(int fd) {
	this->_listenFd = fd;
}

void	Server::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}


void	Server::setCgi(std::string extension, std::string path) {
	this->_cgi[extension] = path;
}

void	Server::setErrorPage(int code, std::string path) {
	this->_error_page[code] = path;
}

void	Server::addLocation(Location location) {
	this->_locations.push_back(location);
}

bool	Server::operator==(Server const &sv) const {
	if (_host == sv._host && _port == sv._port && _server_name == sv._server_name)
		return true;
	return false;
}

bool	Server::operator!=(Server const &sv) const {
	return !(*this == sv);
}

/**
 * Overload << operator
*/

std::ostream &operator<<(std::ostream &os, Server &sv) {
	std::map<int, std::string> error_page = sv.getErrorPages();
	std::map<std::string, std::string> cgi = sv.getCgi();
	std::vector<Location> locations = sv.getLocations();

	os << "  Server" << std::endl;
	os << "    Listen: " << sv.getHostStr() << ":" << sv.getPort() << std::endl;
	os << "    Host: " << sv.getHostStr() << std::endl;
	os << "    Port: " << sv.getPort() << std::endl;
	os << "    Server Name: " << sv.getServerName() << std::endl;
	os << "    Root: " << sv.getRoot() << std::endl;
	os << "    Index: " << sv.getIndex() << std::endl;
	os << "    Allowed Methods: " << sv.getAllowedMethods() << std::endl;
	for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++) {
		os << "    Error Page: " << it->first << " " << it->second << std::endl;
	}
	os << "    Client Max Body Size: " << sv.getClientMaxBodySize() << std::endl;
	os << "    Autoindex: " << sv.getAutoindex() << std::endl;
	for (std::map<std::string, std::string>::iterator it = cgi.begin(); it != cgi.end(); it++) {
		os << "    CGI: " << it->first << " " << it->second << std::endl;
	}
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++) {
		os << *it;
	}
	return os;
}
