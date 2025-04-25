
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
EXPAND_DIR = $(SRC_DIR)/expand
UTILS_DIR = $(SRC_DIR)/utils
TOKEN_DIR = $(SRC_DIR)/tokenizing

BUILTINS    := $(BUILT_DIR)/echo.c $(BUILT_DIR)/cd.c \
					$(BUILT_DIR)/pwd.c $(BUILT_DIR)/export.c \
          			$(BUILT_DIR)/env.c $(BUILT_DIR)/unset.c \
					$(BUILT_DIR)/builtins_utils.c
PARSING     := $(PARSING_DIR)/parse_pipeline.c
EXEC        := $(EXEC_DIR)/exec_ast.c $(EXEC_DIR)/exec_builtins.c \
					$(EXEC_DIR)/exec_path.c

EXPAND      := $(EXPAND_DIR)/expand_arg.c
UTILS       := $(UTILS_DIR)/free.c $(UTILS_DIR)/fd.c $(UTILS_DIR)/error.c\
					$(UTILS_DIR)/str.c
TOKENIZING  := $(TOKEN_DIR)/token.c
COMMON      := $(SRC_DIR)/main.c $(SRC_DIR)/signal.c $(SRC_DIR)/init_ast.c $(SRC_DIR)/print_utils.c $(SRC_DIR)/init_mini.c

SRCS = $(BUILTINS) $(PARSING) $(EXEC) $(EXPAND) $(UTILS) $(TOKENIZING) $(COMMON)

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
