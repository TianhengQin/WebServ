
#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "ASTNode.hpp"


/**
 * @brief Block
 * Represents a block in the nginx configuration file
*/

class Block : public ASTNode {
private:
	std::string					_name;
	std::vector<std::string>	_arguments;
	std::vector<ASTNode * > 	_children;

public:
	Block(const std::string& name, const std::vector<std::string>& arguments);

	void addChild(ASTNode * child);

	std::string					getName(void) const;
	std::vector<std::string>	getArguments(void) const;
	std::vector<ASTNode * >		getChildren(void) const;
	std::vector<ASTNode * >		getServers(void);

	void print(int level = 0) const;
};

#endif // BLOCK_HPP
