#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "head.hpp"

class WebServ {

public:

    WebServ(Config &f);
    ~WebServ();

    void run();

private:

    void initFds();
    void addFd(int fd, char rs);
    void rmFd(int fd, char rs);

    void connect();
    void disconnect();

    void receive();
    void send();

    std::map<int, Server> _servers;
    std::map<int, Connection> _connections;

    fd_set _recvFds;
    fd_set _sendFds;
    int _fd_max;
    int _fd_min;

};

#endif
