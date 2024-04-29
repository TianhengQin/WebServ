
#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "ASTNode.hpp"

// Directive node, represents a single directive
class Directive : public ASTNode {
private:
	std::string                 _name;
	std::vector<std::string>    _arguments;

public:
	Directive(const std::string& name, const std::vector<std::string>& arguments);

	std::string					getName(void) const;
	std::vector<std::string>	getArguments(void) const;

	void print(int level = 0) const override;
};

#endif // DIRECTIVE_HPP
