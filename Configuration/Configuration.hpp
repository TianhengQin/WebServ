
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include "ASTNode.hpp"
# include "Block.hpp"
# include "Directive.hpp"
# include "Parser/NginxParser.hpp"
# include "Server.hpp"
# include "Head.hpp"
# include "Server.hpp"
# include "Location.hpp"

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <ctime>

# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>

# include <vector>
# include <map>
# include <stack>

class Configuration {

public:
	Configuration(void);
	Configuration(const Configuration &other);
	Configuration &operator=(const Configuration &other);
	~Configuration();

	Configuration(std::string filename);

	/* Getters */
	std::string							getFilename(void);
	NginxParser							*getParser(void);
	std::string							getRoot(void);
	std::string							getIndex(void);
	std::map<int, std::string>			getErrorPages(void);
	unsigned int						getClientMaxBodySize(void);
	unsigned int						getAllowedMethods(void);
	bool								getAutoindex(void);
	std::map<std::string, std::string>	getCgi(void);
	std::vector<Server>					getServers(void);

private:
	std::string					_filename;
	NginxParser					*_parser;

	/* Default values */
	std::string							_root;
	std::string							_index;
	std::map<int, std::string>			_error_page;
	unsigned int						_client_max_body_size;
	unsigned int						_allow_methods;
	bool								_autoindex;
	std::map<std::string, std::string>	_cgi;
	std::vector<Server>					_servers;

	/* Private methods */
	void			parse_configuration_file(void);
	void			process_http_block(Block *httpBlock);
	void			process_server_block(Block *serverBlock, Server &server);
	void			process_location_block(Block *locationBlock, Location &location);
	void			set_server_context(Server &server);
	void			set_location_context(Server &server, Location &location);
	void			process_listen_directive(std::vector<std::string> &args, Server &server);

};

std::ostream &operator<<(std::ostream &out, Configuration &config);

#endif

