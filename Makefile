
NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

LIBFT = libft/libft.a
INCLUDES = -Iincludes
LIBS = -lreadline
RM = rm -f

SRC_DIR = src
OBJ_DIR = obj

BUILT_DIR = $(SRC_DIR)/builtins
PARSING_DIR = $(SRC_DIR)/parsing
EXEC_DIR = $(SRC_DIR)/exec
TOKEN_DIR = $(SRC_DIR)/tokenizing

BUILTINS    :=
PARSING     :=
EXEC        := $(EXEC_DIR)/exec_ast.c $(EXEC_DIR)/exec_builtins.c
TOKENIZING  := $(TOKEN_DIR)/token.c
COMMON      := $(SRC_DIR)/main.c $(SRC_DIR)/signal.c $(SRC_DIR)/init_ast.c

SRCS = $(BUILTINS) $(PARSING) $(EXEC) $(TOKENIZING) $(COMMON)

# Replace src/ → obj/ and .c → .o
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

# Create obj/... directory structure and compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
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
