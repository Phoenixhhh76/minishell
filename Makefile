NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

INCLUDES = -Iincludes
LIBS = -lreadline

SRC = src/main.c src/signal.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
