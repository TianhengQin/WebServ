
#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

// # include "../Head.hpp"
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

	std::string			getFilename(void);
	Block				*getASTRoot(void);
	std::vector<Server>	getServs(void);

private:
	std::string			_filename;
	Block				*_ast_root;
	std::vector<Server> _servs;

	void			parseConfig(std::ifstream &file);
	void			setServerConfig(Block *block, Server &server);
	void			processListenDirective(std::vector<std::string> &args, Server &server);
	void			processLocationBlock(ASTNode *locationNode, Server &server);
	int				parseSize(std::string sizeStr);
	unsigned int	parseMethods(std::vector<std::string> &methods);


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
  - Define a list of accepted HTTP methods for the route.
  - Define a HTTP redirection.
  - Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
  - Turn on or off directory listing.
  - Set a default file to answer if the request is a directory.
  - Execute CGI based on certain file extension (for example .php).
  - Make it work with POST and GET methods.
  - Make the route able to accept uploaded files and configure where they should
be saved.
    - Do you wonder what a CGI is?
    - Because you won’t call the CGI directly, use the full path as PATH_INFO.
    - Just remember that, for chunked request, your server needs to unchunk
it, the CGI will expect EOF as end of the body.
    - Same things for the output of the CGI. If no content_length is returned
from the CGI, EOF will mark the end of the returned data.
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