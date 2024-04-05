#include "WebServ.hpp"

WebServ::WebServ(Configuration &conf) {

    FD_ZERO(&_recvFds);
    FD_ZERO(&_sendFds);

    std::vector<Server> servs = conf.getServs();

    std::vector<Server>::iterator it;

    for (it = servs.begin(); it != servs.end(); ++it) {
        it->setup();
        if (listen(it->getFd(), 1024) == -1) {
            Log::print(ERROR, "Listen socket failed on fd ", it->getFd());
            throw std::runtime_error("Listen Failed");
        }
        if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0) {
            Log::print(ERROR, "Set fd nonblocking failed on fd ", it->getFd());
            throw std::runtime_error("Set Nonblock Failed");
        }
        _servers.insert(std::make_pair(it->getFd(), *it));
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
    int ready;

    while (true) {

        FD_COPY(&_recvFds, &recv_dup);
        FD_COPY(&_sendFds, &send_dup);
    
        ready = select(_fdMax+1, &recv_dup, &send_dup, NULL, &timeout);
        if (ready < 0) {
            Log::print(ERROR, "Select failed ", 0);
            throw std::runtime_error("Select Failed");
        } else if (ready == 0) {
            continue;
        }
        for (fd = 0; fd <= _fdMax; ++fd) {
            if (FD_ISSET(fd, &recv_dup)) {
                if (_servers.count(fd)) {
                    connect(fd);
                } else if (_connections.count(fd)) {
                    receive(fd);
                }
            } else if (FD_ISSET(fd, &send_dup) && _connections.count(fd)) {
                send(fd);
            }
        }
        timeOut();
    }
}

void WebServ::timeOut() {
    std::time_t currentTime = std::time(nullptr);
    std::map<int, Connection>::iterator it;
    for (it = _connections.begin(); it != _connections.end(); ++it) {
        if(currentTime - it->second.getTimeStamp() > _timeOut) {
            disconnect(it->first);
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

void WebServ::fdSwitch(int fd, char rs) {
    if (rs == 's') {
        FD_SET(fd, &_recvFds);
        FD_CLR(fd, &_sendFds);
    } else if (rs == 'r') {
        FD_SET(fd, &_sendFds);
        FD_CLR(fd, &_recvFds);
    }
}

void WebServ::connect(int fd) {

    struct sockaddr_in connect_addr;
    unsigned int address_size = sizeof(connect_addr);
    int connect_fd;
    Connection connect(_servers, fd);
    char bf[INET_ADDRSTRLEN];

    connect_fd = accept(fd, (struct sockaddr *)&connect_addr, (socklen_t*)&address_size);
    if (connect_fd == -1) {
        Log::print(ERROR, "Connect failed on fd ", fd);
        return;
    }
    addFd(connect_fd, 'r');
    if (fcntl(connect_fd, F_SETFL, O_NONBLOCK) < 0) {
        Log::print(ERROR, "Set connection nonblock failed on fd ", connect_fd);
        rmFd(connect_fd, 'r');
        close(connect_fd);
        return;
    }
    connect.setFd(connect_fd);
    _connections.insert(std::make_pair(connect_fd, connect));
}

void WebServ::disconnect(int fd) {

    if (FD_ISSET(fd, &_sendFds))
        rmFd(fd, 's');
    if (FD_ISSET(fd, &_recvFds))
        rmFd(fd, 'r');
    close(fd);
    _connections.erase(fd);
}

void WebServ::receive(int fd) {
    char bf[RS_BF_SIZE + 1];
    int received = read(fd, bf, RS_BF_SIZE);
    if (received < 0) {
        Log::print(ERROR, "Read error on fd ", fd);
        disconnect(fd);
    } else if (received == 0) {
        fdSwitch(fd, 'r');
        _connections[fd].buildResponse();
    } else {
        bf[received] = 0;
        _connections[fd].receive(bf);
    }
}

void WebServ::send(int fd) {
    if(_connections[fd].send() == 0) {
        disconnect(fd);
    }
}
