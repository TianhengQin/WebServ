
#include "Directive.hpp"

Directive::Directive(void) {}

Directive::Directive(const std::string& name, const std::vector<std::string>& arguments)
	: _name(name), _arguments(arguments) {}

Directive::Directive(const Directive &other) {
	*this = other;
}

Directive &Directive::operator=(const Directive &other) {
	if (this != &other) {
		this->_name = other._name;
		this->_arguments = other._arguments;
	}
	return (*this);
}

Directive::~Directive() {}


std::string Directive::getName(void) {
	return this->_name;
}

std::vector<std::string> Directive::getArguments(void) {
	return this->_arguments;
}

void Directive::print(int level) const {
	std::string indentation(level * INDENT_SIZE, ' ');
	std::cout << indentation << _name;
	for (std::vector<std::string>::const_iterator arg = _arguments.begin(); arg != _arguments.end(); ++arg) {
		std::cout << " " << *arg;
	}
	std::cout << ";" << std::endl;
}

