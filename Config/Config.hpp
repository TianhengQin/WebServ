#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "head.hpp"

class Config {

public:

    Config();
    ~Config();

    std::vector<Server> &getServs();

private:

    std::vector<Server> _servs;

};

#endif
