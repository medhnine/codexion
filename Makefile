Name = codexion
SRCS = main.c parse.c utils.c
CC = cc -g
flags = -Wall -Wextra -Werror
OBJS = $(SRCS:.c=.o)

all: $(Name)

$(Name): $(OBJS)
	$(CC) $(flags) $(OBJS) -o $(Name)

%.o: %.c header.h
	$(CC) $(flags) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(Name)

re: fclean all

.PHONY: all clean fclean re