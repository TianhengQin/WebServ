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

	void			setup(void);

	/* Getters */
	std::string		&getServerName(void);
	std::string		getRoot(void);
	std::string		getIndex(void);
	std::string		getHostStr(void);
	unsigned int	getHost(void);
	unsigned int	getPort(void);
	unsigned int	getCliMaxBody(void);
	bool			getDefault(void);
	int				getFd(void);

	std::vector<Location>		&getLocations(void);
	std::map<int, std::string>	&get_error_pages(void);

	std::vector<std::string>	&get_all_server_names(void);
	std::vector<std::string>	&get_all_indexes(void);


	/* Setters */
	void	setServerName(std::string name);
	void	setRoot(std::string root);
	void	setIndex(std::string index);
	void	setHost(std::string host);
	void	setPort(unsigned int port);
	void	setCliMaxBody(unsigned int cmb);
	void	setDefault(bool def);
	
	void	setListenFd(int fd);
	void	setErrPage(int code, std::string path);
	void	setLocation(Location &loc);


private:

	void initDefaultErrorPages(void);

	// std::vector<std::string>	_access_logs;

	// std::map<std::string, std::string>	_cgi_param;
	// std::string							_cgi_pass;
	// std::map<std::string, ConfigServer>	_location;
	// std::set<std::string>				_allowed_methods;
	// bool								_autoindex;
	// std::string							_alias;
	// bool								_aliasSet;
	
	/* Server */
	std::string			_servName;
	std::string			_root;
	std::string			_index;
	struct sockaddr_in	_socketAddr;
	unsigned int		_socketAddrLen;
	std::string			_hostStr;
	unsigned int		_host;				// listen
	unsigned int		_port;				// listen
	unsigned int		_cliMaxBody;		// client_max_body_size
	bool				_default;
	int					_listenFd;

	std::map<int, std::string>	_error_page;
	std::vector<Location>		_locations;

	std::vector<std::string>	_server_names;
	std::vector<std::string>	_indexes;

};

std::ostream &operator<<(std::ostream &os, Server &sv);

#endif
