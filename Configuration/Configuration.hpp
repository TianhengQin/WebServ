
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


/*
Configuration file

You can get some inspiration from the ’server’ part of NGINX configuration file.

In the configuration file, you should be able to:
	1. Choose the port and host of each ’server’.
	2. Setup the server_names or not.
	3. The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server).
	4. Setup default error pages.
	5. Limit client body size.
	6. Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
		(1) Define a list of accepted HTTP methods for the route.
		(2) Define a HTTP redirection.
		(3) Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
		(4) Turn on or off directory listing.
		(5) Set a default file to answer if the request is a directory.
		(6) Execute CGI based on certain file extension (for example .php).
		(7) Make it work with POST and GET methods.
		(8) Make the route able to accept uploaded files and configure where they should be saved.
			- Do you wonder what a CGI is?
			- Because you won’t call the CGI directly, use the full path as PATH_INFO.
			- Just remember that, for chunked request, your server needs to unchunk it, the CGI will expect EOF as end of the body.
			- Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.
			- Your program should call the CGI with the file requested as first argument.
			- The CGI should be run in the correct directory for relative path file access.
			- Your server should work with one CGI (php-CGI, Python, and so forth).

You must provide some configuration files and default basic files to test and demonstrate every feature works during evaluation.

If you’ve got a question about one behavior, you should compare your program behavior with NGINX’s.
For example, check how does server_name work.
We’ve shared with you a small tester. It’s not mandatory to pass it if everything works fine with your browser and tests, but it can help you hunt some bugs.

The important thing is resilience. Your server should never die.

Do not test with only one program. Write your tests with a more convenient language such as Python or Golang, and so forth. Even in C or C++ if you want to.

*/
