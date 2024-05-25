
#ifndef NGINXPARSER_HPP
# define NGINXPARSER_HPP

# include "../AST/ASTNode.hpp"
# include "../AST/Block.hpp"
# include "../AST/Directive.hpp"


# include <string>
# include <iostream>
# include <vector>
# include <string>
# include <memory>
# include <fstream>
# include <stack>
# include <algorithm>
# include <cctype>
# include <locale>

# include <iostream>
# include <fstream>
# include <sstream>

class ASTNode;

class NginxParser {
public:
	NginxParser(void);
	NginxParser(const NginxParser &other);
	NginxParser &operator=(const NginxParser &other);
	~NginxParser();

	NginxParser(std::istream& input);

	void			parse(void);
	bool			is_absolute_path(std::string &path);
	unsigned int	parse_size(std::string &str);
	unsigned int 	parse_method(std::vector<std::string> &methods);
	void			trim(std::string &s);
	void			ltrim(std::string &s);
	void			rtrim(std::string &s);


	Block	*getRoot(void) const;
	Block	*getHttpBlock(void) const;
	void	parse_configuration_file(void);

private:

	std::ifstream	_file;
	std::istream&	_input;
	Block			*ast_root;

	std::pair<std::string, std::vector<std::string> >	parseNameAndArguments(const std::string &line);

};

std::ostream &operator<<(std::ostream &out, NginxParser &parser);

#endif // NGINXPARSER_HPP
