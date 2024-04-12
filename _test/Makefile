NAME	= webserv

SRCS	= main.cpp \
		Server/Server.cpp

OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)


$(NAME): $(OBJS)
	@echo "Compiling webserv..."
	@c++ -Wall -Wextra -Werror -std=c++98 -g -o $(NAME) $(SRCS)
	@echo "Done."

clean:
	@echo "Cleaning webserv..."
	@rm -f $(NAME)
	@rm -f $(OBJS)
	@echo "Done."

fclean: clean

re: fclean all

.PHONY: all clean fclean re
