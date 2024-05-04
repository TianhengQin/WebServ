#ifndef SERVER_HPP
# define SERVER_HPP

# include "Head.hpp"
# include "Location.hpp"
// # include "../Log/Log.hpp"

// class Location;

class Server {

public:
	Server(void);
	Server(Server const &sv);
	Server &operator=(Server const &sv);
	~Server();

	void			setup();

	std::string		getName(void);
	std::string		getRoot(void);
	std::string		getIndex(void);
	unsigned int	getHost(void);

	int				getFd(void);
	unsigned int	getPort(void);
	unsigned int	getCliMaxBody(void);

	void	setServName(std::string name);
	void	setRoot(std::string root);
	void	setIndex(std::string index);
	void	setHost(std::string host);
	void	setPort(unsigned int port);
	void	setCliMaxBody(unsigned int cmb);
	void	setErrPage(int code, std::string path);
	void	setLocation(Location &loc);

private:

	void initErrorPages();

	std::vector<std::string>	_server_names;
	std::vector<std::string>	_access_logs;
	std::vector<std::string>	_error_pages;
	std::vector<std::string>	_listen;
	
	std::string		_servName;
	std::string		_root;
	std::string		_index;

	struct sockaddr_in	_socketAddr;
	unsigned int		_socketAddrLen;
	unsigned int		_host;
	unsigned int		_port;
	unsigned int		_cliMaxBody;

	int				_listenFd;

	std::map<int, std::string> _errorPage;

	std::vector<Location> _locations;

};

#endif
