#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>

#define BF_SIZE 4096

class TcpServer {

public:
    TcpServer();
    TcpServer(std::string ip, int port);
    ~TcpServer();

    void beginListen();

private:

    int upServer();
    void downServer();
    void acceptConnect(int &nS);
    std::string Re();
    void sendRe();

    int _socket;
    int _new_socket;
    int _port;
    long _inMsg;
    std::string _ip;
    struct sockaddr_in _socketAddr;
    unsigned int _socketAddr_len;
    std::string _servMsg;
};

#endif
