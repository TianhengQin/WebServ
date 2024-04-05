#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Head.hpp"

class Server {

public:

    Server();
    Server(Server const &sv);
    Server &operator=(Server const &sv);
    ~Server();

    void setup();

    int getFd();
    std::string &getName();
    unsigned int getHost();
    unsigned short getPort();

private:

    void initErrorPages();

    std::string _servName;
    std::string _root;
    std::string _index;

    struct sockaddr_in _socketAddr;
    unsigned int _socketAddrLen;
    unsigned int _host;
    unsigned short _port;
    unsigned int _cliMaxBody;

    int _listenFd;

    std::map<int, std::string> _errorPage;

    std::vector<Location> _locations;

};

#endif
