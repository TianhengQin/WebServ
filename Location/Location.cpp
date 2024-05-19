#include "Location.hpp"

Location::Location(void) {
	_path = "/";
	_root = "/";
	_index = "index.html";
	_redir = "";
	_methods = GET | POST | DELETE | PUT | HEAD;
	_autoindex = false;
}

Location::Location(Location const &loc) {
	*this = loc;
}

Location &Location::operator=(Location const &loc) {
	_path = loc._path;
	_root = loc._root;
	_index = loc._index;
	_redir = loc._redir;
	_methods = loc._methods;
	_autoindex = loc._autoindex;
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
	return (this->_redir);
}

unsigned int Location::getMethods(void) {
	return (this->_methods);
}

bool Location::getAutoindex(void) {
	return (this->_autoindex);
}

std::map<std::string, std::string> Location::getCgi(void) {
	return (this->_cgiMap);
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
	this->_redir = redir;
}

void Location::setMethods(unsigned int methods) {
	this->_methods = methods;
}

void Location::setAutoindex(bool autoindex) {
	this->_autoindex = autoindex;
}

void Location::setCgi(std::string ext, std::string bin) {
	this->_cgiMap[ext] = bin;
}

std::ostream &operator<<(std::ostream &out, Location &loc) {
	out << "    Location: " << loc.getPath() << std::endl;
	out << "      Root: " << loc.getRoot() << std::endl;
	out << "      Index: " << loc.getIndex() << std::endl;
	out << "      Redir: " << loc.getRedir() << std::endl;
	out << "      Methods: " << loc.getMethods() << std::endl;
	out << "      DirListing: " << loc.getAutoindex() << std::endl;
	// for (std::map<std::string, std::string>::const_iterator it = loc._cgiMap.begin(); it != loc._cgiMap.end(); ++it) {
	// 	out << "  CGI: " << it->first << " -> " << it->second << std::endl;
	// }
	return (out);
}
