#ifndef CPPLIB_HPP
# define CPPLIB_HPP

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

// # include "WebServ/WebServ.hpp"
// # include "Connection/Connection.hpp"
// # include "Response/Response.hpp"
// # include "Request/Request.hpp"
// # include "MimeType/MimeType.hpp"
// # include "Cgi/Cgi.hpp"
// # include "Log/Log.hpp"

// # include "Server/Server.hpp"
// // # include "Configuration/Configuration.hpp"
// # include "TestConfig/Configuration.hpp"
// # include "Location/Location.hpp"

# define SELECT_TIMEOUT 1

# define RS_BF_SIZE 4096

int logLevel;

#endif
