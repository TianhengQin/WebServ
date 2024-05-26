#include "Location.hpp"

Location::Location(void) {
	_path = "/";
	_root = "/";
	_index = "index.html";
	// 
	_methods = GET | POST | DELETE | PUT | HEAD;
	_client_max_body_size = 1024 * 1024;
	_autoindex = false;
	_alias = "/";
	// 
	_return = "";
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
		this->_alias = loc._alias;
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

std::map<int, std::string> Location::getErrorPages(void) { return (this->_error_page); }

unsigned int Location::getAllowedMethods(void) { return (this->_methods); }

unsigned int Location::getClientMaxBodySize(void) { return (this->_client_max_body_size); }

bool Location::getAutoindex(void) { return (this->_autoindex); }

std::string Location::getAlias(void) { return (this->_alias); }

std::map<std::string, std::string> Location::getCgi(void) { return (this->_cgi); }

std::string Location::getRedir(void) { return (this->_return); }

/**
 * Special getters
*/

bool Location::hasErrorPage(int code) {
	std::map<int, std::string> error_pages = this->getErrorPages();
	return (error_pages.find(code) != error_pages.end());
}

bool Location::hasCgi(std::string ext) {
	std::map<std::string, std::string> cgi = this->getCgi();
	return (cgi.find(ext) != cgi.end());
}

std::string Location::getErrorPage(int code) {
	std::map<int, std::string> error_pages = this->getErrorPages();
	if (error_pages.find(code) != error_pages.end())
		return (error_pages[code]);
	return ("");
}

std::string Location::getCgi(std::string ext) {
	std::map<std::string, std::string> cgi = this->getCgi();
	if (cgi.find(ext) != cgi.end())
		return (cgi[ext]);
	return ("");
}

bool Location::hasMethod(unsigned int method) {
	return ((this->_methods & method) == method);
}



/**
 * Setters
*/

void Location::setPath(std::string path) { this->_path = path; }

void Location::setRoot(std::string root) { this->_root = root; }

void Location::addIndex(std::string index) { this->_index = index; }

void Location::setErrorPage(int code, std::string path) { this->_error_page[code] = path; }

void Location::setAllowedMethods(unsigned int methods) { this->_methods = methods; }

void Location::setClientMaxBodySize(unsigned int cmb) { this->_client_max_body_size = cmb; }

void Location::setAutoindex(bool autoindex) { this->_autoindex = autoindex; }

void Location::setAlias(std::string alias) { this->_alias = alias; }

void Location::setCgi(std::string ext, std::string bin) { this->_cgi[ext] = bin; }

void Location::setRedir(std::string redir) { this->_return = redir; }


bool Location::operator==(Location const &loc) const {
	return (this->_path == loc._path);
}

bool Location::operator!=(Location const &loc) const {
	return (this->_path != loc._path);
}

/**
 * Overload of the << operator
*/

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
	out << "      Alias: " << loc.getAlias() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it) {
		out << "      CGI: " << it->first << " -> " << it->second << std::endl;
	}
	return (out);
}
