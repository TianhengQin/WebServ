
#ifndef DIRECTIVE_HPP
# define DIRECTIVE_HPP

# include "ASTNode.hpp"

/**
 * @brief Directive
 * Represents a directive in the nginx configuration file
*/

class Directive : public ASTNode {

public:
	Directive(void);
	Directive(const std::string& name, const std::vector<std::string>& arguments);
	Directive(const Directive &other);
	Directive &operator=(const Directive &other);
	~Directive();

	std::string					getName(void);
	std::vector<std::string>	getArguments(void);

	void						print(int level = 0) const;

private:
	std::string					_name;
	std::vector<std::string>	_arguments;

};

std::ostream &operator<<(std::ostream &out, Directive &directive);

#endif // DIRECTIVE_HPP
