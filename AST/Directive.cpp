
#include "Directive.hpp"

Directive::Directive(const std::string& name, const std::vector<std::string>& arguments)
	: _name(name), _arguments(arguments) {}


std::string Directive::getName(void) const {
	return this->_name;
}

std::vector<std::string> Directive::getArguments(void) const {
	return this->_arguments;
}

void Directive::print(int level) const {
	std::string indentation(level * 2, ' ');
	std::cout << indentation << _name;
	for (const auto& arg : _arguments) {
		std::cout << " " << arg;
	}
	std::cout << ";" << std::endl;
}

