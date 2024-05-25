#ifndef HEAD_HPP
# define HEAD_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <ctime>

# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <libgen.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <sys/wait.h>
# include <cstdlib>
# include <cstring>

# include <vector>
# include <map>
# include <stack>

// # include "WebServ.hpp"
// # include "Response.hpp"
// # include "Connection.hpp"

// # include "MimeType.hpp"
// # include "Cgi.hpp"
# include "Log.hpp"

// # include "Server.hpp"
// # include "Configuration.hpp"
// # include "Location.hpp"

# define SELECT_TIMEOUT 4
# define CONNECT_TIMEOUT 8L

# define RS_BF_SIZE 8192

# ifndef DEBUG
#  define DEBUG 0
# endif

#endif
