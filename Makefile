
NAME		= webserver

CXX			= c++
CXXFLAGS	= -std=c++98
CXXFLAGS	+= -Wall -Wextra -Werror
INCLUDE		= -I./AST -I./Configuration/ -I./Server/ -I./Location/ -I./Connection/ -I./Cgi/ \
			-I./Log/ -I./MimeType/ -I./Parser/ -I./Request/ -I./Response/ -I./WebServ/ -I./

SRCS	= main.cpp \
		  AST/ASTNode.cpp AST/Block.cpp AST/Directive.cpp \
		  Configuration/Configuration.cpp \
		  Parser/NginxParser.cpp \
		  Cgi/Cgi.cpp Request/Request.cpp Connection/Connection.cpp Location/Location.cpp Log/Log.cpp \
		  MimeType/MimeType.cpp Response/Response.cpp \
		  Server/Server.cpp WebServ/WebServ.cpp

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

# debug: CXXFLAGS += -g

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
