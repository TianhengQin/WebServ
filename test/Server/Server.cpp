#include "Server.hpp"

TcpServer::TcpServer(std::string ip, int port) : _ip(ip),  _port(port),
    _socket(), _new_socket(), _inMsg(),_socketAddr(), _socketAddr_len(sizeof(_socketAddr)) {
    _socketAddr.sin_family = AF_INET;
    _socketAddr.sin_port = htons(_port);
    _socketAddr.sin_addr.s_addr = inet_addr(_ip.c_str());
    _servMsg = Re();
    upServer();
}

TcpServer::~TcpServer() {
    downServer();
}

int TcpServer::upServer() {

    _socket = socket(AF_INET,SOCK_STREAM,0);
    if(_socket < 0) {
        return 1;
    }
    if(bind(_socket, (sockaddr *)&_socketAddr, _socketAddr_len) < 0) {
        return 1;
    }
    return 0;
}

void TcpServer::downServer() {

    close(_new_socket);
    close(_socket);
    exit(0);
}

void TcpServer::beginListen() {
    if (listen(_socket, 20) < 0) {
        return;
    }
    std::cout << "Listening " << _ip << ":" << _port << std::endl;

    int rcvByte;

    int rqs = 0;
    while(++rqs) {
        std::cout << "Waiting " << rqs << std::endl;
        acceptConnect(_new_socket);

        char bf[BF_SIZE + 1] = {0};
        rcvByte = read(_new_socket, bf, BF_SIZE);
        if (rcvByte < 0) {
            std::cout << "Failed reading" << std::endl;
            return;
        }
        bf[rcvByte] = 0;
        std::cout << "received: "<< bf << std::endl;
        sendRe();
        close(_new_socket);
    }
}

void TcpServer::acceptConnect(int &nS) {

    nS = accept(_socket, (sockaddr *)&_socketAddr, &_socketAddr_len);

    if(nS < 0) {
        std::cout << "Failed accepting" << std::endl;
    }
}

std::string TcpServer::Re() {
    std::string html = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello world </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << html.size() << "\n\n"
        << html;
    return ss.str();
}

void TcpServer::sendRe()
{
    long bytesSent;

    bytesSent = write(_new_socket, _servMsg.c_str(), _servMsg.size());

    if (bytesSent == _servMsg.size()) {
        std::cout << "Sent" << std::endl;
    } else {
        std::cout << "Failed sending" << std::endl;
    }
}
