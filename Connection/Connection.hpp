#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include "../CppLib.hpp"
# include "../Server/Server.hpp"
# include "../Response/Response.hpp"
# include "../Request/Request.hpp"
# include "../Log/Log.hpp"
// class Response;
// class Request;

class Connection {

public:

    Connection();
    Connection(std::map<int, Server> &svs, int fd);
    ~Connection();

    void setFd(int fd);

    void buildResponse();

    void updateTime();

    std::time_t getTimeStamp();

    void receive(char const *bf);
    int send();

private:

    std::map<std::string, Server> _server;

    Request _quest;
    Response _sponse;

    std::time_t _timeStamp;

    std::string _sendBf;

    int _fd;
};

#endif
