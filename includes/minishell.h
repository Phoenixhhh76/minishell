#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

extern pid_t				g_signal_pid;

typedef struct sigaction	t_sigaction;

typedef enum e_node_type
{
	CMD,
	PIPE,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	HEREDOC,
	UNKNOWN = -1
}	t_node_type;

typedef enum e_quote_type {
	QUOTE_NONE = 0,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	t_node_type		type;
	char			*str;
	t_quote_type	quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char	**cmd_args;
	char	*cmd_path;
	char	*infile;
	char	*outfile;
	char	*append;
	int		fd_in;
	int		fd_out;
	int		*pipe;
	int		*pids;
	bool	last_redirin;
	char	**heredocs;
	int		heredoc_nb;
	int		**heredoc_pipe;
	bool	flag_error;
	bool	path_error;
	t_quote_type	*heredocs_quote; // maybe we can change t_quote_type in a shorter name ?
	//int		heredoc_error;//add for exit_error
	//int		child;
}	t_cmd;


typedef struct s_ast
{
	t_token			ast_token;
	int				fd[2];
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*next;
}	t_ast;

typedef struct s_mini
{
	t_ast	*ast;
	t_token	*token;
	char	**env;
	char	**exp_list; // for export without args, not in env
	char	**av;
	int	 	last_exit; // last exit code for $?
	//int		ac;
	//int		heredoc; add in s_cmd
	//???		histoire(readlin add history);
}	t_mini;

//init_mini
void	init_mini(t_mini *mini, char **av, char **env);
char	**copy_env(char **env);

//signals
void	signal_handler(int sig);
void	ft_setup_signals(void);

//tokenizing
void	init_token(t_mini *mini);
t_token	*tokenize_input(const char *input);
void	free_token_list(t_token *token);
t_token	*create_t(char *str, t_quote_type quote_type);
void	append_t(t_token **head, t_token *new);
int		check_unclosed_quotes(const char *line);
bool	check_syntax(t_token *tokens);

//token_helper
int		is_meta_char(char c);
void	skip_spaces(const char *input, int *i);

char	*extract_plain(const char *input, int *i, char *current);
int		handle_meta(const char *input, int i, t_token **tokens);
//extract_quoted.c
char	*extract_quoted(const char *input, int *i, \
							char *current, t_quote_type *qt);

//init_ast
void	init_ast(t_mini *mini);
t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini);

//builtins
int		does_var_exist(char **env, const char *var);
int		find_equal_sign(t_cmd *cmd);
void	free_double_tab(char **tab);

int		ft_echo(t_cmd *cmd); //change void to int for return 0
int		ft_pwd(void);
int		ft_cd(t_cmd *cmd);
int		ft_env(t_cmd *cmd, char ***env);
int		ft_unset(t_cmd *cmd, char ***mini_env);
int		ft_export(t_cmd *cmd, char ***mini_env, t_mini *mini);
void	print_export_env(char **env, char **exp_list);
char	**clone_and_sort_env(char **env);
void	print_sorted_env_line(const char *entry);
int		ft_exit(t_cmd *cmd);


//exec
void	handle_redirects(t_cmd *cmd);
bool	ft_builtin(t_ast *ast);
bool	ft_is_builtin(char *arg);
int		ft_run_builtin(t_cmd *cmd, t_mini *mini);
void	exec_ast(t_ast *node, char **envp);
char	*resolve_cmd_path(char *cmd, char **envp); //add
//char	**get_heredoc(int nb, t_token *start, t_token *end, t_mini *mini);
char	**get_heredoc(int nb, t_token *start, t_token *end, t_cmd *cmd);
//void	check_heredocs(t_ast *node);
void	check_heredocs(t_ast *ast, t_mini *mini);
int		create_heredocs(t_cmd *cmd);
//int		exec_heredocs(t_cmd *cmd);
int		exec_heredocs(t_cmd *cmd, t_mini *mini);
int		**create_heredoc_pipe(int heredoc_nb);
void	close_all_heredocs(t_ast *ast);

//expande
//char	*expand_arg(const char *str, t_mini *mini);
char	*expand_arg(const char *str, t_mini *mini, t_quote_type quote_type);
char	*expand_if_needed(t_token *token, t_mini *mini);
char	*ft_strjoin_f(char *s1, char *s2);
char	*get_env_value(const char *key, char **env);
char	*expand_heredoc_line(const char *str, t_mini *mini);
//char	*expand_var(const char *str, int *i, t_mini *mini);
char	*handle_dollar(const char *str, int *i, char *result, t_mini *mini);
char	*handle_exit_code(char *result, int *i, t_mini *mini);
char	*handle_pid(char *result, int *i);
char	*handle_variable(const char *str, int *i, char *result, t_mini *mini);

// 處理單一普通字元追加到結果字串
char	*append_char(char *result, char c);

//parsing
t_token	*find_next_pipe(t_token *start, t_token *end);
t_ast	*parse_pipeline(t_token *start, t_token *end, t_mini *mini);
t_ast	*create_pipe_node(t_token *start, \
t_token *pipe_pos, t_token *end, t_mini *mini);

//outil
char	*ft_strndup(const char *s, size_t n);
int		ft_isspace(char c);
void	ft_free_char2(char **arr);
void	free_strs(char *str, char **strs);
void	free_split(char **split);
void	close_fds(t_cmd *cmd);
void	exit_error_pipe(int error_status, t_cmd *cmd);
int		err_msg(char *str1, char *str2, char *str3, int erno);
int		syntax_err_msg(char *str1, char *str2, int erno);
void	exit_error(const char *msg);

//tests, to be erased
void	print_tab(char **tab);
void	print_cmd(t_cmd *cmd);
void	print_mini(t_mini *mini);
void	print_token_list(t_token *token);
void	print_ast(t_ast *node, int depth);
#endif
