
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "../CppLib.hpp"

class Configuration {

private:
    std::string         _filename;
    // std::vector<Server> _servs;


public:
    Configuration(void);
    Configuration(std::string filename);

    ~Configuration();

    // std::vector<Server> &getServs(void);


};

#endif
