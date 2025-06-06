/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:12:39 by ndabbous          #+#    #+#             */
/*   Updated: 2025/06/01 11:25:41 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	R_OUT,
	R_IN,
	R_A,
	HD,
	UNKNOWN = -1
}	t_node;

typedef enum e_quote_type
{
	Q_NONE = 0,
	Q_S,
	Q_D
}	t_quote;

typedef struct s_token
{
	t_node			type;
	char			*str;
	t_quote			quote_type;
	bool			is_dollar_quote;
	bool			glued;
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
	bool	flag_hd;
	bool	in_error;
	bool	path_error;
	t_quote	*heredocs_quote;
}	t_cmd;

typedef struct s_ast
{
	t_token			ast_tok;
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
	char	**exp_list;
	char	**av;
	bool	stop_hd;
	int		last_exit;
	int		cpy_in_fd;
	int		cpy_out_fd;
}	t_mini;

typedef struct s_pipe_ctx
{
	t_token	*start;
	t_token	*end;
	t_token	*pipe_pos;
	t_mini	*mini;
	t_ast	*node;
}	t_pipe_ctx;

typedef struct s_parse_state
{
	int			i;
	bool		glued;
	bool		next_glued;
}	t_parse_state;

//init_mini
void	init_mini(t_mini *mini, char **av, char **env);
char	**copy_env(char **env);
t_mini	*static_struct(t_mini *mini);

//signals
void	ft_setup_signals(void);
void	signal_handler(int sig);
void	signal_handler_child(int sig);
void	heredoc_sigint_handler(int sig);

//------tokenizing------//
//token.c//
t_token	*tokenize_input(const char *input, t_mini *mini);
//extract_quoted.c//
bool	has_closing_quote(const char *input, int i, char quote);
bool	is_dollar_quote(const char *input, int i);
char	*extract_quoted(const char *input, int *i, t_quote *qt, t_mini *mini);
//helper.c//
int		is_meta_char(char c);
void	skip_spaces(const char *input, int *i);
char	*extract_plain(const char *input, int *i);
int		handle_meta(const char *input, int i, t_token **tokens);
//syntax_error.c//
int		check_unclosed_quotes(const char *line);
bool	check_syntax(t_token *tokens);
bool	is_redirection(t_node type);
bool	is_meta_token(t_node type);
//token_utils.c//
t_token	*create_t_with_glued(char *str, t_quote qt, bool glued);
void	free_token_list(t_token *token);
t_token	*create_t(char *str, t_quote quote_type);
void	append_t(t_token **head, t_token *new);
char	*handle_special_case(const char *str);

//--------expand--------//
//expand_arg.c//
char	*expand_arg(const char *str, t_mini *mini, \
	t_quote quote_type, bool is_dollar_quote);
char	*expand_if_needed(t_token *token, t_mini *mini);
char	*expand_var(const char *str, int *i, t_mini *mini);
char	*append_char(char *result, char c);
//expand_arg_quote.c//
char	*handle_single_quote(const char *str, int *i);
char	*handle_double_quote(const char *str, int *i, t_mini *mini);
//expand_heredoc.c//
char	*get_env_value(const char *key, char **env);
char	*handle_dollar(const char *str, int *i, char *result, t_mini *mini);
char	*handle_exit_code(char *result, int *i, t_mini *mini);
char	*handle_variable(const char *str, int *i, char *result, t_mini *mini);
char	*expand_heredoc_line(const char *str, t_mini *mini);
bool	handle_quoted_empty_token(char **arg_slot, t_token **start_ptr);
void	process_argument_token(char ***args, \
				int *i, t_token **start_ptr, t_mini *mini);
//-----------parsing-----------//
//init_ast.c//
char	**collect_args(t_token *start, t_token *end, t_mini *mini);
//process_tokens.c//
int		process_token(char **args, int i, t_token *tok, t_mini *mini);
int		handle_single(char **args, int i, t_token *tok);
int		handle_expanded(char **args, int i, t_token *tok, t_mini *mini);
int		add_split(char **args, int i, char *expanded);
//args_count.c//
int		count_expanded_split(char *expanded);
int		count_token_args(t_token *tok, t_mini *mini);
int		count_args_advanced(t_token *start, t_token *end, t_mini *mini);
//export_count.c//
int		count_export_args(t_token *start, t_token *end, t_mini *mini);
void	fill_export_args(char **args, \
					t_token *start, t_token *end, t_mini *mini);
char	*join_tokens_for_arg(t_token **cur_tok_ptr, \
					t_mini *mini, bool allow_split);
char	*collect_tokens(char *arg, t_token **tok, t_mini *mini);
void	fill_current_token(const char *input, \
					t_parse_state *ps, t_token **tokens, t_mini *mini);

//init_ast
void	init_ast(t_mini *mini);

void	handle_redir(t_token *start, t_token *end, t_cmd *cmd, t_mini *mini);
void	handle_redir_in(t_token *tmp, t_cmd *cmd, t_mini *mini);
void	handle_redir_out(t_token *tmp, t_cmd *cmd, t_mini *mini);
void	handle_redir_append(t_token *tmp, t_cmd *cmd, t_mini *mini);
void	count_hd(t_token *tmp, t_cmd *cmd, t_mini *mini);
int		process_token(char **args, int i, t_token *tok, t_mini *mini);
char	**collect_args_for_export(t_token *start, t_token *end, t_mini *mini);
t_cmd	*build_command(t_token *start, t_token *end, t_mini *mini);

//builtins
int		does_var_exist(char **env, const char *var);
int		find_equal_sign(t_cmd *cmd);

int		ft_echo(t_cmd *cmd);
int		ft_pwd(void);
int		ft_cd(t_cmd *cmd);
int		ft_env(t_cmd *cmd, char ***env);
int		ft_unset(t_cmd *cmd, t_mini *mini);
//builtins_utils
int		does_var_exist(char **env, const char *var);
bool	is_authorized_character(char c);
bool	is_valid_var_name(char *var);
int		add_var_to_env(char **env, char *cmd, char ***mini_env);
bool	in_exp_list(char **exp_list, const char *key);
//builtins export
int		ft_export(t_cmd *cmd, char ***mini_env, t_mini *mini);
void	print_export_env(char **env, char **exp_list);
char	**clone_and_sort_env(char **env);
void	print_sorted_env_line(const char *entry);
int		handle_single_export(char *arg, char ***mini_env, t_mini *mini);
void	should_store_unassigned_var(char *arg, char **env, t_mini *mini);
int		handle_var_with_equal(char *arg, \
					char **env, char ***mini_env, t_mini *mini);
char	*strip_quotes_only(const char *val);
char	**split_export_arg(const char *arg);
char	*make_joined_assignment(char **var);
int		handle_var_assignment(char **env, \
					char *joined, char ***mini_env, int index);
void	add_to_exp_list(char ***exp_list, const char *key);
void	remove_from_exp_list(char ***exp_list, const char *key);
//builtin exit
int		ft_exit(t_mini *mini, t_cmd *cmd);
int		args_count(char **args);
bool	ft_isnumeric(char *str);
bool	ft_is_long_limits(const char *nptr);

//exec
void	exec_or_builtin(t_mini *mini);
bool	is_there_pipe(t_mini *mini);
bool	has_redirection(t_cmd *cmd);
void	handle_exit_status(t_mini *mini, int status);

void	handle_redirects(t_cmd *cmd);
bool	ft_builtin(t_ast *ast);
bool	ft_is_builtin(char *arg);
int		ft_run_builtin(t_mini *mini, t_cmd *cmd);

void	exec_ast(t_mini *mini, t_ast *node, char **envp);
void	exec_cmd_node(t_mini *mini, t_ast *node, char **envp);
char	*resolve_cmd_path(char *cmd, char **envp); //add

//heredocs
char	**get_heredoc(int nb, t_token *start, t_token *end, t_cmd *cmd);
void	check_heredocs(t_ast *ast, t_mini *mini);
int		create_heredocs(t_cmd *cmd);
int		fork_heredocs(t_mini *mini, t_cmd *cmd, char *delimiter, int i);
int		exec_heredocs(t_cmd *cmd, t_mini *mini);
int		**create_heredoc_pipe(int heredoc_nb);
void	close_all_heredocs(t_ast *ast);
void	close_all_heredoc_pipes(t_cmd *cmd);

//parsing
t_token	*find_next_pipe(t_token *start, t_token *end);
int		parse_pipeline(t_token *start, t_token *end, t_mini *mini, t_ast *node);
int		handle_pipe_syntax_error(t_ast *node);
int		init_pipe_node(t_ast *node);
int		handle_left_branch(t_token *start, t_token *pipe_pos, \
	t_mini *mini, t_ast *node);
int		handle_right_branch(t_token *pipe_pos, t_token *end, \
	t_mini *mini, t_ast *node);
int		handle_cmd_node(t_token *start, t_token *end, \
	t_mini *mini, t_ast *node);

//--------utils--------//
//utils
char	*ft_strndup(const char *s, size_t n);
char	*append_char(char *result, char c);
int		ft_isspace(char c);
int		begins_with_digits(char *str);

//free.c
void	free_double_tab(char **arr);
void	ft_free_tab_int(int **tab, int size);
void	free_strs(char *str, char **strs);
void	free_split(char **split);
char	*ft_strjoin_f(char *s1, char *s2);
char	*ft_strjoin_ff(char *s1, char *s2);

//utils error.c
int		err_msg(char *str1, char *str2, char *str3, int erno);
int		syntax_err_msg(char *str1, char *str2, int erno);
int		export_err_msg(char *arg, int erno);

//clean.c
void	safe_exit(t_mini *mini, int code);
void	safe_cleanup(t_mini *mini, char *line);
void	free_ast(t_ast *ast);
void	free_cmd(t_cmd *cmd);

//tests, to be erased
void	print_tab(char **tab);
void	print_cmd(t_cmd *cmd);
void	print_mini(t_mini *mini);
void	print_token_list(t_token *token);
void	print_ast(t_ast *node, int depth);
void	debug_tokens_type(t_token *tok);
void	debug_tokens(t_token *tok);

#endif
