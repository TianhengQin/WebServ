
#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "ASTNode.hpp"

/**
 * @brief Directive
 * Represents a directive in the nginx configuration file
*/

class Directive : public ASTNode {
private:
	std::string                 _name;
	std::vector<std::string>    _arguments;

public:
	Directive(const std::string& name, const std::vector<std::string>& arguments);

	std::string					getName(void) const override;
	std::vector<std::string>	getArguments(void) const override;

	void print(int level = 0) const override;
};

#endif // DIRECTIVE_HPP
