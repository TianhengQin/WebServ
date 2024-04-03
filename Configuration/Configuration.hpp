
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "head.hpp"

class Config {

private:
    std::string         _filename;
    std::vector<Server> _servs;


public:
    Config(void);
    Config(std::string filename);

    ~Config();

    std::vector<Server> &getServs(void);


};

#endif
