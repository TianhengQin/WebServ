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
    return *this;
}

Location::~Location() {}

std::string &Location::getPath() {
    return _path;
}

std::string &Location::getRoot() {
    return _root;
}

std::string &Location::getIndex() {
    return _index;
}

std::string &Location::getRedir() {
    return _redir;
}

int Location::getMethods() {
    return _methods;
}

bool Location::getDirListing() {
    return _dirListing;
}

void Location::setPath(std::string const pth) {
    _path = pth;
}

void Location::setRoot(std::string const rt) {
    _root = rt;
}

void Location::setIndex(std::string const idx) {
    _index = idx;
}

void Location::setRedir(std::string const rdr) {
    _redir = rdr;
}

void Location::setMethods(int mtd) {
    _methods = mtd;
}

void Location::setDirListing(bool dl) {
    _dirListing = dl;
}

void Location::setCgi(std::string const ext, std::string const bin) {
    _cgiMap[ext] = bin;
}
