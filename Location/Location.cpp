#include "Location.hpp"

Location::Location(void) {
	_path = "/";
	_root = "/";
	_index = "index.html";
	_return = "";
	_methods = GET | POST | DELETE | PUT | HEAD;
	_autoindex = false;
	_client_max_body_size = 1024 * 1024;
}

Location::Location(Location const &loc) {
	*this = loc;
}

Location &Location::operator=(Location const &loc) {
	_path = loc._path;
	_root = loc._root;
	_index = loc._index;
	_return = loc._return;
	_methods = loc._methods;
	_autoindex = loc._autoindex;
	_cgi = loc._cgi;
	return (*this);
}

Location::~Location() {}

/**
 * Getters
*/

std::string Location::getPath(void) {
	return (this->_path);
}

std::string Location::getRoot(void) {
	return (this->_root);
}

std::string Location::getIndex(void) {
	return (this->_index);
}

std::string Location::getRedir(void) {
	return (this->_return);
}

unsigned int Location::getAllowedMethods(void) {
	return (this->_methods);
}

bool Location::getAutoindex(void) {
	return (this->_autoindex);
}

unsigned int Location::getClientMaxBodySize(void) {
	return (this->_client_max_body_size);
}

std::map<int, std::string> Location::getErrorPages(void) {
	return (this->_error_page);
}

std::map<std::string, std::string> Location::getCgi(void) {
	return (this->_cgi);
}

/**
 * Setters
*/

void Location::setPath(std::string path) {
	this->_path = path;
}

void Location::setRoot(std::string root) {
	this->_root = root;
}

void Location::setIndex(std::string index) {
	this->_index = index;
}

void Location::setRedir(std::string redir) {
	this->_return = redir;
}

void Location::setAllowedMethods(unsigned int methods) {
	this->_methods = methods;
}

void Location::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}

void Location::setClientMaxBodySize(unsigned int cmb) {
	this->_client_max_body_size = cmb;
}

void Location::setCgi(std::string extension, std::string script) {
	this->_cgi[extension] = script;
}

std::ostream &operator<<(std::ostream &out, Location &loc) {
	out << "    Location: " << loc.getPath() << std::endl;
	out << "      Root: " << loc.getRoot() << std::endl;
	out << "      Index: " << loc.getIndex() << std::endl;
	out << "      Redir: " << loc.getRedir() << std::endl;
	out << "      Methods: " << loc.getAllowedMethods() << std::endl;
	out << "      DirListing: " << loc.getAutoindex() << std::endl;
	// for (std::map<std::string, std::string>::const_iterator it = loc._cgi.begin(); it != loc._cgi.end(); ++it) {
	// 	out << "  CGI: " << it->first << " -> " << it->second << std::endl;
	// }
	return (out);
}
