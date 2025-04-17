/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:39:45 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/17 10:54:11 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>

extern pid_t	g_signal_pid;

typedef enum e_node_type
{
    NODE_COMMAND,
    NODE_PIPE,
    NODE_REDIR_OUT,
    NODE_REDIR_IN,
    NODE_REDIR_APPEND,
    NODE_HEREDOC,
}   t_node_type;

typedef struct	s_token
{
	t_node_type	token;
	char		*str;
	t_token		*next;
}	t_token;


typedef struct	s_cmd
{
	char	**cmd_args;
	char	**cmd_opts;
	char	*cmd_path;
	int		fd_in;
	int		fd_out;
	char	*infile;
	char	*outfile;
	int		*pipe;
	int		*pids;
	//int		child;
}	t_cmd;

typedef struct	s_ast
{
	t_token	token;

	int			fd[2];
	t_cmd		*left;
	t_cmd		*right;
	t_ast		*next;
}	t_ast;

typedef struct s_mini
{
	t_ast	*ast;
	t_token	*token;
	char	**env;
	char	**av;
	//int		ac;
	//int		heredoc;
	//???		histoire(readlin add history);

	//struct t_mini *next;
}	t_mini;

#endif