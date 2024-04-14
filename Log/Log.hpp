#ifndef LOG_HPP
# define LOG_HPP

# include "Head.hpp"

# define DEBUG 0
# define INFO 1
# define WARNING 2
# define ERROR 3

# define LEVEL INFO

namespace Log {

    void print(int level, std::string const &msg, unsigned long i);

};

#endif
