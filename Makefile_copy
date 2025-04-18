NAME = minishell

LIBFT = libft/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror

INCLUDES = -Iincludes
LIBS = -lreadline

REMOVE = rm -f


BUILTINS	:=	builtins/cd.c \
				builtins/echo.c \
				builtins/env_utils.c \
				builtins/env.c \
				builtins/exit.c \
				builtins/export.c \
				builtins/pwd.c \
				builtins/unset.c

TOKENIZING	:=	tokenizing/token_appenders.c \
				tokenizing/token_handlers.c \
				tokenizing/token_utils.c \
				tokenizing/token.c

EXEC		:=	exec/error_msg.c \
				exec/exec_builtin.c \
				exec/exec_redirect.c \
				exec/exec_utils.c \
				exec/exec.c \
				exec/exist_check.c \
				exec/ft_exec_simple_cmd.c \
				exec/ft_get_path.c \
				exec/init_tree.c

PARSING		:=	parsing/parser_clear.c \
				parsing/parser_err.c \
				parsing/parser_helpers.c \
				parsing/parser_nodes.c \
				parsing/parser_utils.c \
				parsing/parser.c

SRCS		:=	$(BUILTINS)\
				$(EXEC)\
				$(PARSING)\
				$(TOKENIZING)\
				main.c signal.c token.c init_ast.c

OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C libft

clean:
	$(REMOVE) $(OBJS)
	@$(MAKE) -C libft clean

fclean: clean
	$(REMOVE) $(NAME)
	@$(MAKE) -C libft fclean

re: fclean all

.PHONY: all clean fclean re
