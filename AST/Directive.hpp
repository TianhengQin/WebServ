
#pragma once

# include "ASTNode.hpp"

// Directive node, represents a single directive
class Directive : public ASTNode {
private:
	std::string                 _name;
	std::vector<std::string>    _arguments;

public:
	Directive(const std::string& name, const std::vector<std::string>& arguments)
		: _name(name), _arguments(arguments) {}

	void print(int level = 0) const override {
		std::string indentation(level * 2, ' ');
		std::cout << indentation << _name;
		for (const auto& arg : _arguments) {
			std::cout << " " << arg;
		}
		std::cout << ";" << std::endl;
	}
};
