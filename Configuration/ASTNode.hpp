
#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <vector>
# include <string>
# include <memory>
# include <iostream>


// Base class for any node in the AST
class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void print(int level = 0) const = 0;
};

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

// Block node, represents a block that can contain directives or other blocks
class Block : public ASTNode {
private:
    std::string                             _name;
    std::vector<std::string>                _arguments;
    std::vector<ASTNode * >                 _children;

public:
    Block(const std::string& name, const std::vector<std::string>& arguments)
        : _name(name), _arguments(arguments) {}

    void addChild(ASTNode * child) {
        _children.push_back(child);
    }

    void print(int level = 0) const override {
        std::string indentation(level * 2, ' ');
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
};

#endif // AST_HPP
