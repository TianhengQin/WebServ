#ifndef HEAD_HPP
# define HEAD_HPP

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

// class WebServ;
// class Server;
// class Connection;
// class Location;
// class Response;
// class Request;
// class Configuration;
// class MimeType;
// class Cgi;

# include "Response/Response.hpp"
# include "Request/Request.hpp"
# include "MimeType/MimeType.hpp"
# include "Location/Location.hpp"
# include "Server/Server.hpp"
# include "Connection/Connection.hpp"
# include "Cgi/Cgi.hpp"
# include "Log/Log.hpp"
# include "Configuration/Configuration.hpp"
# include "WebServ/WebServ.hpp"

# define SELECT_TIMEOUT 1

#endif
