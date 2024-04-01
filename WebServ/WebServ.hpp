#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "head.hpp"

# define RS_BF_SIZE 4096

class WebServ {

public:

    WebServ(Config &conf);
    ~WebServ();

    void run();

private:

    void addFd(int fd, char rs);
    void rmFd(int fd, char rs);
    void fdSwitch(int fd, char rs);

    void connect(int fd);
    void disconnect(int fd);

    void receive(int fd);
    void send(int fd);
    void timeOut();

    void recvCgi();
    void sendCgi();

    std::map<int, Server> _servers;
    std::map<int, Connection> _connections;

    fd_set _recvFds;
    fd_set _sendFds;
    int _fdMax;

    std::time_t _timeOut;

};

#endif
