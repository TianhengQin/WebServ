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

	std::string		&getName(void);
	std::string		getRoot(void);
	std::string		getIndex(void);
	unsigned int	getHost(void);
	std::string		&getHostStr(void);

	int				getFd(void);
	unsigned int	getPort(void);
	unsigned int	getCliMaxBody(void);

	void	setServName(std::string name);
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
	// // std::vector<std::string>	_listen; _host, _port

	// std::map<std::string, std::string>	_cgi_param;
	// std::string							_cgi_pass;
	// std::map<std::string, ConfigServer>	_location;
	// std::set<std::string>				_allowed_methods;
	// bool								_autoindex;
	// std::string							_alias;
	// bool								_aliasSet;
	// // static  ConfigServer				_defaultServer;
	
	std::string					_servName;
	// std::vector<std::string>	_server_names;
	std::string					_root;
	std::string					_index;
	// std::vector<std::string>	_indexes;


	struct sockaddr_in	_socketAddr;
	unsigned int		_socketAddrLen;
	unsigned int		_host;				// listen
	unsigned int		_port;				// listen
	unsigned int		_cliMaxBody;		// client_max_body_size
	std::string			_hostStr;
	bool				_default;

	int					_listenFd;

	std::map<int, std::string> _error_page;

	std::vector<Location> _locations;

};

#endif

/*

class ConfigServer {
	public:
		ConfigServer(void);
		ConfigServer(ConfigServer const &src);
		virtual ~ConfigServer(void);

		ConfigServer						&operator=(ConfigServer const &src);
		int									parseServer(unsigned int &i, std::vector<std::string> &file);
		void								passMembers(ConfigServer &server) const;

		class	ExceptionInvalidArguments: public std::exception {
			virtual const char	*what() const throw();
		};

		static ConfigServer					_initDefaultServer(const char *filename);

		// GETERS
		std::vector<t_listen>				getListen() const;
		std::string							getRoot() const;
		std::vector<std::string>   			getServerName() const;
		std::map<int, std::string>			getErrorPage() const;
		int									getClientBodyBufferSize() const;
		std::map<std::string, std::string>	getCgiParam() const;
		std::string							getCgiPass() const;
		std::map<std::string, ConfigServer> getLocation() const;
		std::set<std::string>				getAllowedMethods() const;
		std::vector<std::string>			getIndex() const;
		bool								getAutoIndex() const;
		std::string							getAlias() const;
		bool								getAliasSet() const;
		static ConfigServer					&getDefaultServer();

		// RETURN CONFIG ACCORDING TO URI
		ConfigServer						getLocationForRequest(std::string const path, std::string &locationPath);

		friend	std::ostream &operator<<(std::ostream &out, const ConfigServer &server);

	private:
		int				parseLocation(unsigned int &i, std::vector<std::string> &file);
		// ADD MEMBER FUNCTIONS
		void    							addListen(std::vector<std::string> args);
		void    							addRoot(std::vector<std::string> args);
		void    							addServerName(std::vector<std::string> args);
		void    							addErrorPage(std::vector<std::string> args);
		void    							addClientBodyBufferSize(std::vector<std::string> args);
		void								addCgiParam(std::vector<std::string> args);
		void    							addCgiPass(std::vector<std::string> args);
		void								addAllowedMethods(std::vector<std::string> args);
		void								addIndex(std::vector<std::string> args);
		void								addAutoIndex(std::vector<std::string> args);
		void								addAlias(std::vector<std::string> args);

		// MEMBERS
		std::vector<t_listen>				_listen;
		std::string							_root;


		std::vector<std::string>   			_server_name;
		std::map<int, std::string>			_error_page; // error page redirections
		int									_client_body_buffer_size; // max size for the client body, defaults to 8 000
		std::map<std::string, std::string>	_cgi_param;
		std::string							_cgi_pass;
		std::map<std::string, ConfigServer>	_location;
		std::set<std::string>				_allowed_methods;
		std::vector<std::string>			_index;
		bool								_autoindex;
		std::string							_alias;
		bool								_aliasSet;
		static  ConfigServer				_defaultServer;
		static	parseMap					serverParsingMap;
		static	parseMap					locationParsingMap;
		static parseMap 					_initServerMap();
		static parseMap 					_initLocationMap();


};

*/
