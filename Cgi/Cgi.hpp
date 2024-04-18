#ifndef CGI_HPP
# define CGI_HPP

# include "Head.hpp"

class Cgi {

public:

    Cgi();
    ~Cgi();

    int getConnectFd();

private:

    int _connectFd;
    int _pipeIn[2];
    int _pipeOut[2];
    int pid;

};

#endif
