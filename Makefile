NAME = webserv
CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++98
INCLUDE = webserv.hpp
SRCS = 
OBJS = $(SRCS:.cpp=.o)

.SILENT:
all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(INCLUDE)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
