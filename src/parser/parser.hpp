
#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <fstream>
# include <string>
# include <map>

class Parser {
private:
    std::string filename;
    std::map < std::string , std::string > data;

public:
    Parser(const std::string& filename);
    ~Parser();

    void parse();
    void printData() const;
};

#endif
