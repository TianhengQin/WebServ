#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "Head.hpp"
# include "Server.hpp"
# include "Response.hpp"
# include "MimeType.hpp"
# include "Request.hpp"

class Connection {

public:

    Connection();
    Connection(std::map<int, Server> &svs, int fd);
    ~Connection();

    void setFd(int fd);

    void buildResponse();

    void updateTime();

    std::time_t getTimeStamp();

    void receive(char const *bf, size_t rcvd);
    int send();

private:

    std::map<std::string, Server> _server;

    Request _quest;
    Response _sponse;

    MimeType _mmty;

    std::time_t _timeStamp;

    std::string _sendBf;

    int _fd;
};

#endif
