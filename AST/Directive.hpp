
#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "ASTNode.hpp"

/**
 * @brief Directive
 * Represents a directive in the nginx configuration file
*/

class Directive : public ASTNode {
private:
	std::string					_name;
	std::vector<std::string>	_arguments;

public:
	Directive(const std::string& name, const std::vector<std::string>& arguments);
	~Directive();

	std::string					getName(void);
	std::vector<std::string>	getArguments(void);

	void print(int level = 0) const;
};

#endif // DIRECTIVE_HPP
