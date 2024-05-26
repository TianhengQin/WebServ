#include "Webserver.hpp"

WebServ::WebServ(Configuration &conf) {

	_timeOut = CONNECT_TIMEOUT;

	FD_ZERO(&_recvFds);
	FD_ZERO(&_sendFds);

	std::vector<Server> servs = conf.getServers();

	std::vector<Server>::iterator it;
	std::vector<Server>::iterator itr;
	std::vector<Server>::iterator its;
	int same_nb;

	for (it = servs.begin(); it != servs.end(); ++it) {
		same_nb = 0;
		for (itr = servs.begin(); itr != it; ++itr) {
			if (it->getHost() == itr->getHost() && it->getPort() == itr->getPort()) {
				if (same_nb == 0) {
					its = itr;
				}
				++same_nb;
			}
		}
		if (same_nb) {
			_servers.insert(std::make_pair(its->getFd() + 1024 * same_nb, *it));
			continue;
		}
		it->setup();
		_servers.insert(std::make_pair(it->getFd(), *it));
		if (listen(it->getFd(), 1024) == -1) {
			Log::print(ERROR, "Listen socket failed on fd ", it->getFd());
			throw std::runtime_error("Listen Failed");
		}
		if (fcntl(it->getFd(), F_SETFL, O_NONBLOCK) < 0) {
			Log::print(ERROR, "Set fd nonblocking failed on fd ", it->getFd());
			throw std::runtime_error("Set Nonblock Failed");
		}
		FD_SET(it->getFd(), &_recvFds);
		_fdMax = it->getFd();
	}
}

WebServ::~WebServ() {
	std::map<int, Connection>::iterator itcon;
	for (itcon = _connections.begin(); itcon != _connections.end(); ++itcon) {
		close(itcon->first);
	}
	std::map<int, Cgi>::iterator itcgi;
	for (itcgi = _cgis.begin(); itcgi != _cgis.end(); ++itcgi) {
		itcgi->second.kill();
	}
	std::map<int, Server>::iterator itsv;
	for (itsv = _servers.begin(); itsv != _servers.end(); ++itsv) {
		close(itsv->first);
	}
}

WebServ::WebServ(void) { }
WebServ::WebServ(WebServ const &ws) { (void)ws; }
WebServ &WebServ::operator=(WebServ const &ws) { (void)ws; return *this; }


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
			Log::print(ERROR, "Select failed ", ready);
			throw std::runtime_error("Select Failed");
		} else if (ready == 0) {
			timeOut();
			continue;
		}
		for (fd = 0; fd <= _fdMax; ++fd) {
			if (FD_ISSET(fd, &recv_dup)) {
				if (_servers.count(fd)) {
					connect(fd);
				} else if (_connections.count(fd)) {
					receive(fd);
				} else if (_cgiFds.count(fd)) {
					recvCgi(fd);
				}
			} else if (FD_ISSET(fd, &send_dup)) {
				if (_connections.count(fd) && _connections[fd].cgiState() != CGI_ON) {
					send(fd);
				} else if (_cgiFds.count(fd)) {
					sendCgi(fd);
				}
			}
		}
		timeOut();
	}
}

void WebServ::timeOut() {
	std::vector<int> timeOutList;
	std::time_t currentTime = std::time(NULL);
	std::map<int, Connection>::iterator it;
	for (it = _connections.begin(); it != _connections.end(); ++it) {
		if(currentTime - it->second.getTimeStamp() > _timeOut) {
			timeOutList.push_back(it->first);
		}
	}
	if (timeOutList.empty()) {
		return;
	}
	std::vector<int>::iterator itt;
	for (itt = timeOutList.begin(); itt != timeOutList.end(); ++itt) {
		if (_connections[*itt].cgiState() == CGI_ON) {
			closeCgi(_cgis[*itt].getPipeInFd(), CGI_TIMEOUT);
			return;
		}
		disconnect(*itt);
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

	if (rs == 'r' && FD_ISSET(fd, &_recvFds)) {
		FD_CLR(fd, &_recvFds);
	} else if (rs == 's' && FD_ISSET(fd, &_sendFds)) {
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
	if (_connections[fd].cgiState() == CGI_ON) {
		closeCgi(_cgis[fd].getPipeInFd(), CGI_ON);
	}
	_connections.erase(fd);
}

void WebServ::receive(int fd) {
	char bf[RS_BF_SIZE + 1];
	int received = read(fd, bf, RS_BF_SIZE);
	if (received < 0) {
		Log::print(ERROR, "Read error on fd ", fd);
		disconnect(fd);
	} else if (received < RS_BF_SIZE) {
		_connections[fd].receive(bf, received);
		received = read(fd, bf, RS_BF_SIZE);
		if (received > 0) {
			_connections[fd].receive(bf, received);
			return;
		}
		fdSwitch(fd, 'r');
		_connections[fd].buildResponse();
		if (_connections[fd].cgiState() == CGI_ON) {
			openCgi(fd);
		}
	} else {
		_connections[fd].receive(bf, received);
		Log::print(INFO, "Received ", received);
	}
}

void WebServ::send(int fd) {
	if(_connections[fd].send() == 0) {
		disconnect(fd);
	}
}

void WebServ::openCgi(int fd) {

	Cgi cgi(_connections[fd]);

	if (_connections[fd].cgiState() == CGI_FAILED) {
		_connections[fd].buildCgiResponse("fail");
		return;
	}
	addFd(cgi.getPipeInFd(), 's');
	addFd(cgi.getPipeOutFd(), 'r');
	_cgis.insert(std::make_pair(fd, cgi));
	_cgiFds[cgi.getPipeInFd()] = fd;
	_cgiFds[cgi.getPipeOutFd()] = fd;
	_cgis[fd].run(_connections[fd]);
}

void WebServ::closeCgi(int fd, int state) {
	int inFd = _cgis[_cgiFds[fd]].getPipeInFd();
	int outFd = _cgis[_cgiFds[fd]].getPipeOutFd();
	int conn = _cgis[_cgiFds[fd]].getConnectFd();
	rmFd(inFd, 's');
	rmFd(outFd, 'r');
	close(inFd);
	close(outFd);
	if (state == CGI_TIMEOUT) {
		_cgis[_cgiFds[fd]].kill();
	}
	_cgis.erase(_cgiFds[fd]);
	_cgiFds.erase(outFd);
	_cgiFds.erase(inFd);
	_connections[conn].setCgiState(state);
}

void WebServ::recvCgi(int fd) {
	int re = _cgis[_cgiFds[fd]].receive();
	if (re < 0) {
		closeCgi(fd, CGI_FAILED);
	} else if (re == 0) {
		if (_cgis[_cgiFds[fd]].end()) {
			closeCgi(fd, CGI_FAILED);
			return;
		}
		int conn = _cgis[_cgiFds[fd]].getConnectFd();
		_connections[conn].buildCgiResponse(_cgis[_cgiFds[fd]].response());
		rmFd(fd, 'r');
		closeCgi(fd, CGI_SUCCEED);
	}
}

void WebServ::sendCgi(int fd) {
	if (_cgis[_cgiFds[fd]].send() == 0) {
		close(fd);
		rmFd(fd, 's');
	}
}
