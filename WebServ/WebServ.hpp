#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "Head.hpp"
# include "Configuration.hpp"
# include "Server.hpp"
# include "Connection.hpp"
# include "Cgi.hpp"

class WebServ {

public:

	WebServ(Configuration &conf);
	~WebServ();

	void run();

private:
	WebServ(void);
	WebServ(WebServ const &ws);
	WebServ &operator=(WebServ const &ws);

	void	addFd(int fd, char rs);
	void	rmFd(int fd, char rs);
	void	fdSwitch(int fd, char rs);

	void	connect(int fd);
	void	disconnect(int fd);

	void	receive(int fd);
	void	send(int fd);
	void	timeOut();

	void	openCgi(int fd);
	void	closeCgi(int fd, int state);
	void	recvCgi(int fd);
	void	sendCgi(int fd);

	std::map<int, Server> 		_servers;
	std::map<int, Connection>	_connections;
	std::map<int, int>			_cgiFds;
	std::map<int, Cgi>			_cgis;

	fd_set _recvFds;
	fd_set _sendFds;
	int _fdMax;

	std::time_t _timeOut;

};

#endif
