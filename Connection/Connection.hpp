#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "Head.hpp"
# include "Server.hpp"
# include "Response.hpp"
# include "MimeType.hpp"
# include "Request.hpp"

# define CGI_ON 1
# define CGI_OFF 0
# define CGI_FAILED -1

class Connection {

public:

    Connection();
    Connection(std::map<int, Server> &svs, int fd);
    ~Connection();

    void setFd(int fd);
    int getFd();

    void buildResponse();
    void setResponse(std::string &bf);

    void updateTime();

    std::time_t getTimeStamp();

    void receive(char const *bf, size_t rcvd);
    int send();
    bool session();
    int cgiState();
    void setCgiState(int s);
    void buildCgiResponse();

private:

    std::map<std::string, Server> _server;

    Request _quest;
    Response _sponse;

    MimeType _mmty;

    std::time_t _timeStamp;

    std::string _sendBf;
    std::string _cgiSendBf;

    int _fd;

    bool _keepAlive;

    int _cgiState;

    std::string _cgiProgram;
};

#endif
