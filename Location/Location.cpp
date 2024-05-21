#include "Location.hpp"

Location::Location(void) {
	_path = "/";
	_root = "/";
	_index = "index.html";
	_return = "";
	_methods = GET | POST | DELETE | PUT | HEAD;
	_autoindex = false;
	_client_max_body_size = 1024 * 1024;
	// _error_page[400] = "";
	// _cgi[".py"] = "/usr/bin/php-cgi";
}

Location::Location(Location const &loc) {
	*this = loc;
}

Location &Location::operator=(Location const &loc) {
	if (this != &loc) {
		this->_path = loc._path;
		this->_root = loc._root;
		this->_index = loc._index;
		this->_return = loc._return;
		this->_methods = loc._methods;
		this->_autoindex = loc._autoindex;
		this->_client_max_body_size = loc._client_max_body_size;
		this->_error_page = loc._error_page;
		this->_cgi = loc._cgi;
	}
	return (*this);
}

Location::~Location() {}

/**
 * Getters
*/

std::string Location::getPath(void) { return (this->_path); }

std::string Location::getRoot(void) { return (this->_root); }

std::string Location::getIndex(void) { return (this->_index); }

std::string Location::getRedir(void) { return (this->_return); }

unsigned int Location::getAllowedMethods(void) { return (this->_methods); }

bool Location::getAutoindex(void) { return (this->_autoindex); }

unsigned int Location::getClientMaxBodySize(void) { return (this->_client_max_body_size); }

std::map<int, std::string> Location::getErrorPages(void) { return (this->_error_page); }

std::map<std::string, std::string> Location::getCgi(void) { return (this->_cgi); }

/**
 * Setters
*/

void Location::setPath(std::string path) { this->_path = path; }

void Location::setRoot(std::string root) { this->_root = root; }

void Location::addIndex(std::string index) { this->_index = index; }

void Location::setRedir(std::string redir) { this->_return = redir; }

void Location::setAllowedMethods(unsigned int methods) { this->_methods = methods; }

void Location::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }

void Location::setClientMaxBodySize(unsigned int cmb) { this->_client_max_body_size = cmb; }

void Location::setErrorPage(int code, std::string path) { this->_error_page[code] = path; }

void Location::setCgi(std::string extension, std::string script) { this->_cgi[extension] = script; }


std::ostream &operator<<(std::ostream &out, Location &loc) {
	std::map<int, std::string> error_pages = loc.getErrorPages();
	std::map<std::string, std::string> cgi = loc.getCgi();

	out << "    Location: " << loc.getPath() << std::endl;
	out << "      Root: " << loc.getRoot() << std::endl;
	out << "      Index: " << loc.getIndex() << std::endl;
	for (std::map<int, std::string>::const_iterator it = error_pages.begin(); it != error_pages.end(); ++it) {
		out << "      ErrorPage: " << it->first << " -> " << it->second << std::endl;
	}
	out << "      Methods: " << loc.getAllowedMethods() << std::endl;
	out << "      ClientMaxBodySize: " << loc.getClientMaxBodySize() << std::endl;
	out << "      Autoindex: " << loc.getAutoindex() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it) {
		out << "      CGI: " << it->first << " -> " << it->second << std::endl;
	}
	return (out);
}
