
# The program name must be "webserv".
# Todo: Change the WebServ folder name
# NAME	= webserv 

NAME	= webserver

CC		= c++
CFLAGS	= -std=c++98
# CFLAGS	+= -Wall -Wextra -Werror
INCLUDE	= -I./Head.hpp -I./Configuration/Configuration.hpp


SRCS	= main.cpp \
		  Configuration/Configuration.cpp 

OBJS	= $(SRCS:.cpp=.o)


%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE)  -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
