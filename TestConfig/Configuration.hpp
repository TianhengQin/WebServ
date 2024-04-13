
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "Head.hpp"
# include "Server.hpp"
# include "Location.hpp"

class Configuration {

private:

    std::vector<Server> _servs;

public:
    Configuration(void);

    ~Configuration();

    std::vector<Server> &getServs(void);

};

#endif
