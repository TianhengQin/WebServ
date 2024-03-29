#ifndef LOG_HPP
# define LOG_HPP

# include "head.hpp"

# define DEBUG 0
# define INFO 1
# define WARNING 2
# define ERROR 3

int logLevel;

namespace Log {

    void print(int level, std::string const &msg);

};

#endif
