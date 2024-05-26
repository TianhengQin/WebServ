#ifndef SERVER_HPP
# define SERVER_HPP

# include "Head.hpp"
# include "Location.hpp"

class Server {

public:

	Server(void);
	Server(Server const &sv);
	Server &operator=(Server const &sv);
	~Server();

	void			setup(void);

	/* Getters for private members */
	std::string							getHostStr(void);
	unsigned int						getHost(void);
	unsigned int						getPort(void);
	std::string							getServerName(void);
	std::string							getRoot(void);
	std::string							getIndex(void);
	unsigned int						getAllowedMethods(void);
	std::map<int, std::string>			getErrorPages(void);
	unsigned int						getClientMaxBodySize(void);
	bool								getAutoindex(void);
	std::map<std::string, std::string>	getCgi(void);
	int									getFd(void);
	std::vector<Location>				getLocations(void);
	std::vector<std::string>			get_allServerNames(void);
	std::vector<std::string>			get_allIndexes(void);


	/* Setters */
	void			setHost(std::string host);
	void			setPort(unsigned int port);
	void			addServerName(std::string name);
	void			setRoot(std::string root);
	void			addIndex(std::string index);
	void			setAllowedMethods(unsigned int methods);
	void			setErrorPage(int code, std::string path);
	void			setClientMaxBodySize(unsigned int client_max_body_size);
	void			setAutoindex(bool autoindex);
	void			setCgi(std::string extension, std::string path);
	void			setListenFd(int fd);
	void			addLocation(Location location);
	/* Server methods */
	bool	operator==(Server const &sv) const;
	bool	operator!=(Server const &sv) const;
private:

	void initDefaultErrorPages(void);

	/* Server simple directives */
	std::string							_listen;
	unsigned int						_host;
	unsigned int						_port;
	std::string							_server_name;
	std::string							_root;
	std::string							_index;
	unsigned int						_allow_methods;
	std::map<int, std::string>			_error_page;
	unsigned int						_client_max_body_size;
	bool								_autoindex;
	std::map<std::string, std::string>	_cgi;
	std::vector<Location>				_locations;

	struct sockaddr_in					_socket_address;
	unsigned int						_socket_address_length;
	int									_listenFd;
	std::vector<std::string>			_all_server_names;
	std::vector<std::string>			_all_index;


};

std::ostream &operator<<(std::ostream &os, Server &sv);

#endif
