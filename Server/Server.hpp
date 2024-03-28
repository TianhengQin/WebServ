#ifndef SERVER_HPP
# define SERVER_HPP

# include "head.hpp"

class Server {

public:

    Server();
    ~Server();

private:

    std::string _servName;
    std::string _root;
    std::string _index;

    struct sockaddr_in _socketAddr;
    unsigned int _socketAddr_len;
    unsigned int _host;
    unsigned short _port;
    unsigned int _cliMaxBody;

    std::map<int, std::string> _errorPage;

    std::vector<Location> _locations;

};

#endif
