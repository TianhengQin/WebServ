#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "../Head.hpp"

class Connection {

public:

    Connection(std::map<int, Server> &svs, int fd);
    ~Connection();

    void setFd(int fd);

    void buildResponse();

    void updateTime();

    std::time_t getTimeStamp();

    void receive(std::string const &bf);
    int send();

private:

    std::map<std::string, Server> _server;

    MimeType _mime;

    Request _quest;
    Response _sponse;

    std::time_t _timeStamp;

    std::string _sendBf;

    int _fd;
};

#endif
