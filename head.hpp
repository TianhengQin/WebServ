#ifndef HEAD_HPP
# define HEAD_HPP

# include <string>
# include <iostream>
# include <cstdlib>

# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>

# include <vector>
# include <map>

# include "WebServ/WebServ.hpp"
# include "Server/Server.hpp"
# include "Connection/Connection.hpp"
# include "Location/Location.hpp"
# include "Response/Response.hpp"
# include "Request/Request.hpp"
# include "Config/Config.hpp"
# include "MimeType/MimeType.hpp"
# include "Cgi/Cgi.hpp"
# include "Log/Log.hpp"

# define SELECT_TIMEOUT 1

#endif
