
# The program name must be "webserv".
# Todo: Change the WebServ folder name
# NAME	= webserv 

NAME	= webserver

CC		= c++
CFLAGS	= -std=c++98
# CFLAGS	+= -Wall -Wextra -Werror
INCLUDE	= -I./Head.hpp


SRCS	= main.cpp \
		  Configuration/Configuration.cpp 

OBJS	= $(SRCS:.cpp=.o)



all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
