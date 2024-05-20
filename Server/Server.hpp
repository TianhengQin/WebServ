#ifndef SERVER_HPP
# define SERVER_HPP

# include "Head.hpp"
# include "Location.hpp"
// # include "../Log/Log.hpp"

class Server {

public:

	Server(void);
	Server(Server const &sv);
	Server &operator=(Server const &sv);
	~Server();

	void			setup(void);

	/* Getters */
	std::string		getServerName(void);
	std::string		getRoot(void);
	std::string		getIndex(void);
	std::string		getHostStr(void);
	unsigned int	getHost(void);
	unsigned int	getPort(void);
	unsigned int	getClientMaxBodySize(void);
	bool			getDefault(void);
	int				getFd(void);
	bool			getAutoindex(void);

	std::map<int, std::string>	getErrorPages(void);
	std::vector<std::string>	get_allServerNames(void);
	std::vector<std::string>	get_allIndexes(void);
	std::vector<Location>		getLocations(void);



	/* Setters */
	void	setServerName(std::string name);
	void	setRoot(std::string root);
	void	setIndex(std::string index);
	void	setHost(std::string host);
	void	setPort(unsigned int port);
	void	setClientMaxBodySize(unsigned int cmb);
	void	setDefault(bool def);
	void	setListenFd(int fd);
	void	setAutoindex(bool ai);
	void	setErrorPage(int code, std::string path);
	void	setLocation(Location &loc);


private:

	void initDefaultErrorPages(void);

	// std::map<std::string, ConfigServer>	_location;
	// bool								_autoindex;
	// std::string							_alias;
	// bool								_aliasSet;
	
	/* Server */
	std::string			_server_name;
	std::string			_root;
	std::string			_index;
	struct sockaddr_in	_socket_address;
	unsigned int		_socket_address_length;
	std::string			_listen;
	unsigned int		_host;
	unsigned int		_port;
	unsigned int		_client_max_body_size;
	bool				_default;
	int					_listenFd;
	bool				_autoindex;

	std::map<int, std::string>	_error_page;
	std::vector<Location>		_locations;

	std::vector<std::string>	_all_server_names;
	std::vector<std::string>	_all_index;

};

std::ostream &operator<<(std::ostream &os, Server &sv);

#endif
