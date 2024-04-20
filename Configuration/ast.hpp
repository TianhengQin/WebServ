#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <stream>


// Base class for any node in the AST
class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual void print(int level = 0) const = 0; // Pure virtual function for polymorphic printing
};

// Directive node, represents a single directive
class Directive : public ASTNode {
private:
    std::string name;
    std::vector<std::string> arguments;

public:
    Directive(const std::string& name, const std::vector<std::string>& arguments)
        : name(name), arguments(arguments) {}

    void print(int level = 0) const override {
        std::string indent(level * 2, ' '); // Create an indentation string
        std::cout << indent << name;
        for (const auto& arg : arguments) {
            std::cout << " " << arg;
        }
        std::cout << ";" << std::endl;
    }
};

// Block node, represents a block that can contain directives or other blocks
class Block : public ASTNode {
private:
    std::string name;
    std::vector<std::string> arguments;
    std::vector<std::shared_ptr<ASTNode>> children;

public:
    Block(const std::string& name, const std::vector<std::string>& arguments)
        : name(name), arguments(arguments) {}

    void addChild(std::shared_ptr<ASTNode> child) {
        children.push_back(child);
    }

    void print(int level = 0) const override {
        std::string indent(level * 2, ' '); // Create an indentation string
        std::cout << indent << name;
        for (const auto& arg : arguments) {
            std::cout << " " << arg;
        }
        std::cout << " {" << std::endl;

        for (const auto& child : children) {
            child->print(level + 1); // Print children with increased indentation
        }

        std::cout << indent << "}" << std::endl;
    }
};
