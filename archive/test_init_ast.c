#include "../includes/test.h"

static int	process_token(const char *input, int i, t_token **tokens);
static int	process_quoted_token(const char *input, int i, t_token **tokens);
static int	process_redirection(const char *input, int i, t_token **tokens);

t_ast *create_pipe_node(t_token *start, t_token *pipe_pos, t_token *end);

void	*ft_calloc(size_t count, size_t size)
{
	void	*new;

	if (size != 0 && count > (SIZE_MAX / size))
		return (NULL);
	new = malloc(count * size);
	if (!new)
		return (NULL);
	ft_bzero(new, count * size);
	return (new);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
void	*ft_memset(void *s, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

static t_node	get_token_type(const char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, ">"))
		return (R_OUT);
	if (!strcmp(str, "<"))
		return (R_IN);
	if (!strcmp(str, ">>"))
		return (R_A);
	if (!strcmp(str, "<<"))
		return (HD);
	return (CMD);
}

static t_token	*create_t(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str);
	new->next = NULL;
	return (new);
}

static void	append_t(t_token **head, t_token *new)
{
	t_token	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

char	*ft_strdup(const char *s1)
{
	char	*new;
	size_t	i;

	new = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

t_token	*tokenize_input(const char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		i = process_token(input, i, &tokens);
	}
	return (tokens);
}

static int	process_quoted_token(const char *input, int i, t_token **tokens)
{
	char	quote; //we need to take care about "" '' later
	int		start;

	quote = input[i];
	start = ++i;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
	{
		append_t(tokens, create_t(ft_strndup(&input[start], i - start)));
		return (i + 1); // 跳過右引號
	}
	// if we cant fint the seconde quote, we make token as well
	append_t(tokens, create_t(ft_strndup(&input[start - 1], i - start + 1)));
	return (i);
}

static int	process_redirection(const char *input, int i, t_token **tokens)
{
	int	len;

	len = 1;
	if (input[i] == input[i + 1])
		len = 2;
	append_t(tokens, create_t(ft_strndup(&input[i], len)));
	return (i + len);
}

static int	process_token(const char *input, int i, t_token **tokens)
{
	int	start;

	if (input[i] == '"' || input[i] == '\'')
		return (process_quoted_token(input, i, tokens));
	else if (input[i] == '>' || input[i] == '<')
		return (process_redirection(input, i, tokens));
	else if (input[i] == '|')
	{
		append_t(tokens, create_t(ft_strndup(&input[i], 1)));
		return (i + 1);
	}
	else
	{
		start = i;
		while (input[i] && !ft_isspace(input[i])
			&& input[i] != '|' && input[i] != '<' && input[i] != '>'
			&& input[i] != '"' && input[i] != '\'')
			i++;
		append_t(tokens, create_t(ft_strndup(&input[start], i - start)));
		return (i);
	}
}


void	free_token_list(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
}

void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s Type: %d\n", token->str, token->type);
		token = token->next;
	}
}

static int	count_args(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == -1)
			count++;
		start = start->next;
	}
	return (count);
}

static char	**collect_args(t_token *start, t_token *end)
{
	int		i;
	int		size;
	char	**args;

	i = 0;
	size = count_args(start, end);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == -1)
		{
			args[i] = ft_strdup(start->str);
			i++;
		}
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

t_cmd	*build_command(t_token *start, t_token *end)
{
	t_cmd	*cmd;
	t_token	*tmp;

	if (start == end) // 💥 關鍵保護：空指令不建 command
	return NULL;
	tmp = start;
	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	//if (!node || !cmd)
		//ERROR, return (NULL);
	cmd->fd_in = -1; //to be determined;
	cmd->fd_out = -1; //to be determined;
	while (tmp && tmp != end)
	{
		if (tmp->type == R_IN && tmp->next)
		{
			cmd->infile = ft_strdup(tmp->next->str);
			tmp = tmp->next;
		}
		else if (tmp->type == R_OUT && tmp->next)
		{
			cmd->outfile = ft_strdup(tmp->next->str);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == R_A && tmp->next)
		{
			cmd->outfile = ft_strdup(tmp->next->str);
			cmd->fd_out = STDOUT_FILENO;
			tmp = tmp->next;
		}
		else if (tmp->type == HD && tmp->next) // à gérer plus tard
		{
			cmd->infile = ft_strdup(tmp->next->str);//need a tempfile
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	cmd->cmd_args = collect_args(start, end);
	if (cmd->cmd_args && cmd->cmd_args[0])
		cmd->cmd_path = ft_strdup(cmd->cmd_args[0]);
	return (cmd);
}

void	ft_ast_addback(t_ast **type, t_ast *new)
{
	t_ast	*tmp;

	if (!(*type))
	{
		*type = new;
		return ;
	}
	tmp = *type;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/* replace by parse_pipeline() + create_pipe_node()

t_ast	*create_ast(t_token *pipe_pos, t_token *mini_token)
{
	t_ast	*ast;

	ast = (t_ast *)ft_calloc(sizeof(t_ast), 1);
	if (!ast)
		return (NULL);
	ast->ast_token.type = PIPE;
	ast->ast_token.str = ft_strdup("|");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->left = build_command(mini_token, pipe_pos);
	ast->right = build_command(pipe_pos->next, NULL);
	ast->next = NULL;
	return (ast);
} */

t_token	*find_next_pipe(t_token *start, t_token *end)
{
	while (start && start != end)
	{
		if (start->type == PIPE)
			return (start);
		start = start->next;
	}
	return (NULL);
}

t_ast	*parse_pipeline(t_token *start, t_token *end)
{
	t_token	*pipe_pos;
	t_ast	*ast;

	if (start == end)
	return NULL;
	pipe_pos = find_next_pipe(start, end);
	if (pipe_pos)
		return (create_pipe_node(start, pipe_pos, end));

	// case of no PIPE, just one CMD
	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->ast_token.type = CMD;
	ast->ast_token.str = ft_strdup("CMD");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->cmd = build_command(start, end);
	return (ast);
}


t_ast	*create_pipe_node(t_token *start, t_token *pipe_pos, t_token *end)
{
	t_ast	*ast;
	t_ast	*right_ast;

	if (!start || start == pipe_pos || !pipe_pos->next || pipe_pos->next == end)
	{
		fprintf(stderr, "syntax error near unexpected pipe\n");
		return NULL;
	}
	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->ast_token.type = PIPE;
	ast->ast_token.str = ft_strdup("|");
	ast->fd[0] = -1;
	ast->fd[1] = -1;
	ast->left = parse_pipeline(start, pipe_pos);
	if (!ast->left)
	{
		fprintf(stderr, "syntax error: empty command before pipe\n");
		free(ast);
		return NULL;
	}

	// recursion
	right_ast = parse_pipeline(pipe_pos->next, end);
	// if (right_ast && right_ast->left)
	// 	ast->right = right_ast->left;
	// else if (right_ast)
	// 	ast->right = build_command(pipe_pos->next, end);
	if (!right_ast)
	{
		fprintf(stderr, "syntax error: invalid command after pipe\n");
		free(ast);
		return NULL;
	}
	ast->right = parse_pipeline(pipe_pos->next, end);
	return (ast);
}


void	init_ast(t_mini *mini)
{
	mini->ast = parse_pipeline(mini->token, NULL);
}

/* -----test------- */
/*
 cc test_init_ast.c -lreadline
 ls -al

echo hello | cat -e

ls -al | grep ".c" | wc -l

echo a > a.txt | cat < a.txt

exit
*/

// --- AST print tool
void print_ast(t_ast *ast, int level)
{
	if (!ast)
		return;

	for (int i = 0; i < level; i++) printf("  ");
	printf("Node: %-10s (type: %d)\n",
		ast->ast_token.str ? ast->ast_token.str : "(null)",
		ast->ast_token.type);

	if (ast->cmd)
	{
		for (int i = 0; i < level + 1; i++) printf("  ");
		printf("Command: ");
		for (int i = 0; ast->cmd->cmd_args && ast->cmd->cmd_args[i]; i++)
			printf("%s ", ast->cmd->cmd_args[i]);
		printf("\n");
	}

	if (ast->left)
	{
		for (int i = 0; i < level; i++) printf("  ");
		printf("Left:\n");
		print_ast(ast->left, level + 1);
	}
	if (ast->right)
	{
		for (int i = 0; i < level; i++) printf("  ");
		printf("Right:\n");
		print_ast(ast->right, level + 1);
	}
}


int main(void)
{
	char	*input;
	t_token	*tokens;
	t_mini	mini;

	while (1)
	{
		input = readline("minishell> ");
		if (!input || !*input)
			continue;
		if (!strcmp(input, "exit"))
			break;

		add_history(input);
		tokens = tokenize_input(input);
		if (!tokens)
		{
			printf("tokenize error\n");
			free(input);
			continue;
		}

		// 初始化 mini 結構
		mini.token = tokens;
		mini.ast = NULL;
		mini.env = NULL;
		mini.av = NULL;

		init_ast(&mini);

		printf("\n AST :\n");
		print_ast(mini.ast, 0);

		// TODO: free_token_list(tokens);
		// TODO: free_ast(mini.ast);
		free(input);
	}
	return 0;
}