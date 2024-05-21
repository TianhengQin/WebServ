
#ifndef BLOCK_HPP
# define BLOCK_HPP

# include "ASTNode.hpp"


/**
 * @brief Block
 * Represents a block in the nginx configuration file
*/

class Block : public ASTNode {
public:
	Block(void);
	Block(const std::string& name, const std::vector<std::string>& arguments);
	Block(const Block &other);
	Block &operator=(const Block &other);
	~Block();

	void addChild(ASTNode * child);

	std::string					getName(void);
	std::vector<std::string>	getArguments(void);
	std::vector<ASTNode * >		getChildren(void);

	void print(int level = 0) const;

private:
	std::string					_name;
	std::vector<std::string>	_arguments;
	std::vector<ASTNode * > 	_children;


};

#endif // BLOCK_HPP
