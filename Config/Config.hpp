
#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "head.hpp"

class Config {

private:
    std::vector<Server> _servs;


public:
    Config(void);
    Config(std::string filename);

    ~Config();

    std::vector<Server> &getServs(void);


};

#endif
