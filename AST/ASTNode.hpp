
#ifndef ASTNODE_HPP
# define ASTNODE_HPP

# include <vector>
# include <string>
# include <memory>
# include <iostream>

# define INDENT_SIZE 4

/**
 * @brief Abstract Syntax Tree Node
 Base class for any node in the abstract syntax tree
*/


class ASTNode {
public:
	virtual ~ASTNode() {}

	// virtual std::string					getName(void) const = 0;
	// virtual std::vector<std::string>	getArguments(void) const = 0;
	virtual void						print(int level = 0) const = 0;
};


#endif // AST_HPP
