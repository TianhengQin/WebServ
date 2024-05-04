#ifndef CGI_HPP
# define CGI_HPP

# include "Head.hpp"
# include "Connection.hpp"

class Cgi {

public:

    Cgi();
    Cgi(Connection &conn);
    ~Cgi();

    // int setup(Connection &conn);
    int run(Connection &conn);
    int end();
    int getConnectFd();
    int getPipeOutFd();
    int getPipeInFd();

    int send();
    int receive();

    void setRequestBody(std::string &rb);
    std::string &response();

private:

    void setEnv();
    void exeCgi();

    int _connectFd;
    int _pipeIn[2];
    int _pipeOut[2];
    int _pid;

    std::string _sendBf;
    std::string _recvBf;
    std::string _program;
    std::string _script;

    std::vector<std::string> _env;
};

#endif
