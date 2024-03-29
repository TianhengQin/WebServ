#include "WebServ.hpp"

WebServ::WebServ(Config &conf) {

    FD_ZERO(&_recvFds);
    FD_ZERO(&_sendFds);

    std::vector<Server> servs = conf.getServs();

    std::vector<Server>::iterator it;

    for (it = servs.begin(); it != servs.end(); ++it) {
        it->setup();
        _servers.insert(std::pair(it->getFd(), *it));
        FD_SET(it->getFd(), &_recvFds);
    }

    _fdMax = servs.back().getFd();
}


WebServ::~WebServ() {}

void WebServ::run() {

    fd_set recv_dup; 
    fd_set send_dup;

    struct timeval timeout;
    timeout.tv_sec = SELECT_TIMEOUT;
    timeout.tv_usec = 0;

    int fd;

    while (true) {

        FD_COPY(&_recvFds, &recv_dup);
        FD_COPY(&_sendFds, &send_dup);
        int ready = select(_fdMax+1, &recv_dup, &send_dup, NULL, &timeout);

        if (ready < 0) {
            Log::print(ERROR, "Select failed");
            throw std::runtime_error("Select Failed");
        } else if (ready == 0) {
            continue;
        }
        for (fd = 0; fd <= _fdMax; ++fd) {
            if (FD_ISSET(fd, &recv_dup) && _servers.count(fd)) {
                connect(fd);
                receive(fd);
            } else if (FD_ISSET(fd, &send_dup) && _connections.count(fd)) {
                send(fd);
                disconnect(fd);
            }
        }
    }
}

void WebServ::addFd(int fd, char rs) {
    if (rs == 'r') {
        FD_SET(fd, &_recvFds);
    } else if (rs == 's') {
        FD_SET(fd, &_sendFds);
    }
    if (fd > _fdMax) {
        _fdMax = fd;
    }
}

void WebServ::rmFd(int fd, char rs) {
    if (rs == 'r') {
        FD_CLR(fd, &_recvFds);
    } else if (rs == 's') {
        FD_CLR(fd, &_sendFds);
    }
    if (fd == _fdMax) {
        --fd;
        while(!FD_ISSET(fd, &_recvFds) && !FD_ISSET(fd, &_sendFds)) {
            --fd;
        }
        _fdMax = fd;
    }
}

void WebServ::connect(int fd) {

}

void WebServ::disconnect(int fd) {

}
