#include "Location.hpp"

Location::Location() {
	_path = "/";
	_root = "/";
	_index = "index.html";
	_redir = "";
	_methods = GET | POST | DELETE | PUT | HEAD;
	_dirListing = false;
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
	_dirListing = loc._dirListing;
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

int Location::getMethods(void) {
	return (this->_methods);
}

bool Location::getDirListing(void) {
	return (this->_dirListing);
}

std::map<std::string, std::string> Location::getCgi(void) {
	return (this->_cgiMap);
}

/**
 * Setters
*/

void Location::setPath(std::string pth) {
	this->_path = pth;
}

void Location::setRoot(std::string rt) {
	this->_root = rt;
}

void Location::setIndex(std::string idx) {
	this->_index = idx;
}

void Location::setRedir(std::string rdr) {
	this->_redir = rdr;
}

void Location::setMethods(int mtd) {
	this->_methods = mtd;
}

void Location::setDirListing(bool dl) {
	this->_dirListing = dl;
}

void Location::setCgi(std::string ext, std::string bin) {
	this->_cgiMap[ext] = bin;
}

std::ostream &operator<<(std::ostream &out, Location &loc) {
	out << "Location: " << loc.getPath() << std::endl;
	out << "  Root: " << loc.getRoot() << std::endl;
	out << "  Index: " << loc.getIndex() << std::endl;
	out << "  Redir: " << loc.getRedir() << std::endl;
	out << "  Methods: " << loc.getMethods() << std::endl;
	out << "  DirListing: " << loc.getDirListing() << std::endl;
	// for (std::map<std::string, std::string>::const_iterator it = loc._cgiMap.begin(); it != loc._cgiMap.end(); ++it) {
	// 	out << "  CGI: " << it->first << " -> " << it->second << std::endl;
	// }
	return (out);
}
