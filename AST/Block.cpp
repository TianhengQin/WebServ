
#include "Block.hpp"

Block::Block(void) {}

Block::Block(const std::string& name, const std::vector<std::string>& arguments)
	: _name(name), _arguments(arguments) {}

Block::Block(const Block &other) {
	*this = other;
}

Block &Block::operator=(const Block &other) {
	if (this != &other) {
		this->_name = other._name;
		this->_arguments = other._arguments;
		this->_children = other._children;
	}
	return (*this);
}

Block::~Block() {
	for (std::vector<ASTNode *>::iterator child = _children.begin(); child != _children.end(); ++child) {
		delete *child;
	}
}

void Block::addChild(ASTNode * child) {
	this->_children.push_back(child);
}

std::string Block::getName(void) {
	return this->_name;
}

std::vector<std::string> Block::getArguments(void) {
	return this->_arguments;
}

std::vector<ASTNode * > Block::getChildren(void) {
	return this->_children;
}

void Block::print(int level) const {
	std::string indentation(level * INDENT_SIZE, ' ');
	std::cout << indentation << _name;
	for (std::vector<std::string>::const_iterator arg = _arguments.begin(); arg != _arguments.end(); ++arg) {
		std::cout << " " << *arg;
	}
	std::cout << " {" << std::endl;
	for (std::vector<ASTNode *>::const_iterator child = _children.begin(); child != _children.end(); ++child) {
		(*child)->print(level + 1);
	}
	std::cout << indentation << "}" << std::endl;
}

