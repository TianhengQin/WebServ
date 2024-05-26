#ifndef HEAD_HPP
# define HEAD_HPP


// to compile in linux:
// # ifndef FD_COPY
// #  define FD_COPY(f, t)   __DARWIN_FD_COPY(f, t)
// #  define __DARWIN_FD_COPY(f, t)  bcopy(f, t, sizeof(*(f)))
// # endif /* FD_COPY */


# include <string>
# include <cstring>
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

# include <vector>
# include <map>
# include <stack>

// # include "Webserver.hpp"
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
