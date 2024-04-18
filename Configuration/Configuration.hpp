
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

// # include "../Head.hpp"
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <ctime>

# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>

# include <vector>
# include <map>
# include <stack>

class Configuration {

private:
    std::string                         _filename;
    std::map<std::string, std::string>  _config;
    // std::vector<Server> _servs;


public:
    Configuration(void);
    Configuration(std::string filename);

    ~Configuration();

    // std::vector<Server> &getServs(void);


};

#endif
