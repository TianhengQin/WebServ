
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

class ASTNode;

class NginxParser {
public:	
	NginxParser(void);
	NginxParser(const NginxParser &other);
	NginxParser &operator=(const NginxParser &other);
	~NginxParser();

	NginxParser(std::istream& input);

	void	parse(void);
	Block	*getRoot(void) const;


	void	trim(std::string &s);
	void	ltrim(std::string &s);
	void	rtrim(std::string &s);

private:
	std::istream&		_input;
	Block				*_root;

	std::pair<std::string, std::vector<std::string> >	parseNameAndArguments(const std::string &line);

};

#endif // NGINXPARSER_HPP
