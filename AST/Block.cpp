
#include "Block.hpp"

Block::Block(const std::string& name, const std::vector<std::string>& arguments)
	: _name(name), _arguments(arguments) {}


void Block::addChild(ASTNode * child) {
	this->_children.push_back(child);
}

std::string Block::getName(void) const {
	return this->_name;
}

std::vector<std::string> Block::getArguments(void) const {
	return this->_arguments;
}

std::vector<ASTNode * > Block::getChildren(void) const {
	return this->_children;
}

void Block::print(int level) const {
	std::string indentation(level * INDENT_SIZE, ' ');
	std::cout << indentation << _name;
	for (const auto& arg : _arguments) {
		std::cout << " " << arg;
	}
	std::cout << " {" << std::endl;
	for (const auto& child : _children) {
		child->print(level + 1);
	}
	std::cout << indentation << "}" << std::endl;
}

// Replaced in Configuration.cpp
std::vector<ASTNode * > Block::getServers(void) {
	std::vector<ASTNode *> servers;
	for (const auto& child : this->_children) {
		if (child->getName() == "server") {
			servers.push_back(child);
		}
	}
	return servers;
}
