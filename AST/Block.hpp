
#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "ASTNode.hpp"


// Block node, represents a block that can contain directives or other blocks
class Block : public ASTNode {
private:
	std::string					_name;
	std::vector<std::string>	_arguments;
	std::vector<ASTNode * > 	_children;

public:
	Block(const std::string& name, const std::vector<std::string>& arguments);

	void addChild(ASTNode * child);

	std::string					getName(void) const override;
	std::vector<std::string>	getArguments(void) const override;
	std::vector<ASTNode * >		getChildren(void) const;
	std::vector<ASTNode * >		getServers(void);

	void print(int level = 0) const override;
};

#endif // BLOCK_HPP
