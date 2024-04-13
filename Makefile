
# The program name must be "webserv".
# Todo: Change the WebServ folder name
# NAME	= webserv 

NAME	= webserver

CXX		= c++
CXXFLAGS= -std=c++98
CXXFLAGS+= -Wall -Wextra -Werror
INCLUDE	= -I./TestConfig/ -I./Server/ -I./Location/ -I./Connection/ -I./Cgi/ \
		  -I./Log/ -I./MimeType/ -I./Request/ -I./Response/ -I./WebServ/ -I./

# SRCS	= main.cpp \
# 		  Configuration/Configuration.cpp \

SRCS	= main.cpp \
		  TestConfig/Configuration.cpp \
		  Cgi/Cgi.cpp Request/Request.cpp Connection/Connection.cpp Location/Location.cpp Log/Log.cpp \
		  MimeType/MimeType.cpp Response/Response.cpp \
		  Server/Server.cpp WebServ/WebServ.cpp

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re