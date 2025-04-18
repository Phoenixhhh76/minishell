NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

LIBFT = libft/libft.a
INCLUDES = -Iincludes
LIBS = -lreadline

RM = rm -f

SRC_DIR = src
OBJ_DIR = obj
BUILT_DIR = builtins
PARSING_DIR = parsing
EXEC_DIR = exec
TOKENIZING_DIR = tokenizing

#SRCS files
#BUILTINS    := $(BUILT_DIR)/builtin1.c $(BUILT_DIR)/builtin2.c
#PARSING     := $(PARSING_DIR)/parser1.c $(PARSING_DIR)/parser2.c
#EXEC        := $(EXEC_DIR)/exec1.c $(EXEC_DIR)/exec2.c
TOKENIZING  := $(TOKENIZING_DIR)/token.c
SRCS        := $(BUILTINS) $(PARSING) $(EXEC) $(TOKENIZING) $(SRC_DIR)/main.c $(SRC_DIR)/signal.c $(SRC_DIR)/init_ast.c

OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C libft

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C libft clean

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re
