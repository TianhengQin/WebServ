
#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <vector>
# include <string>
# include <memory>
# include <iostream>

#define INDENT_SIZE 4

// Base class for any node in the AST
class ASTNode {
public:
	virtual ~ASTNode() {}
	virtual void print(int level = 0) const = 0;
};


#endif // AST_HPP
