
#include "Block.hpp"
#include "Directive.hpp"

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
	std::vector<ASTNode *>::iterator child;
	for (child = _children.begin(); child != _children.end(); ++child) {
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


std::ostream &operator<<(std::ostream &out, Block &block) {
	std::string inden(INDENT_SIZE, ' ');
	out << inden << "<" << block.getName() << "> -> {";
	std::vector<std::string> args = block.getArguments();
	for (std::vector<std::string>::iterator arg = args.begin(); arg != args.end(); ++arg) {
		out << *arg;
		// if (arg + 1 != args.end()) {
			out << ", ";
		// }
	}
	
	std::vector<ASTNode *> children = block.getChildren();
	for (std::vector<ASTNode *>::iterator child = children.begin(); child != children.end(); ++child) {
		Block *block = dynamic_cast<Block*>(*child);
		if (block) {
			out << *block;
		} else {
			Directive *directive = dynamic_cast<Directive*>(*child);
			if (directive) {
				out << *directive;
			}
		}
		// out << (*child);
	}
	
	out << "}" << std::endl;
	return out;
}
