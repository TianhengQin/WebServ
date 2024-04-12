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

    void setServName(std::string const &nam);
    void setRoot(std::string const &rt);
    void setIndex(std::string const &idx);
    void setHost(std::string const &hst);
    void setPort(unsigned short pt);
    void setCliMaxBody(unsigned int cmb);
    void setErrPage(int code, std::string const &path);
    void setLocation(Location &loc);

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
