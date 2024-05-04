
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

void	ltrim(std::string &s);
void	rtrim(std::string &s);
void	trim(std::string &s);

class Block *	parseConfig(std::istream& input);

#endif // NGINXPARSER_HPP
