/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellOld.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:39:45 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:12:23 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELLOLD_H
# define MINISHELLOLD_H

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
# include "../libft/libft.h"

extern pid_t	g_signal_pid;

typedef enum e_node_type
{
	CMD,
	PIPE,
	R_OUT,
	R_IN,
	R_A,
	HD,
}	t_node;

typedef struct s_token
{
	t_node		type;
	char			*str;
	struct s_token	*next;
}	t_token;

// typedef struct s_ast
// {
// 	t_token	type;
// 	char	**cmd_args;
// 	char	**cmd_opts;
// 	char	*cmd_path;
// 	char	*infile;
// 	char	*outfile;
// 	int		fd_in;
// 	int		fd_out;
// 	int		*pipe;
// 	int		*pids;
// 	//int		child;
// 	//int		fd[2];
// 	t_ast	*next;
// }	t_ast;

typedef struct s_cmd
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

typedef struct s_ast
{
	t_token			ast_token;
	int				fd[2];
	t_cmd			*left;
	t_cmd			*right;
	struct s_ast	*next;
}	t_ast;

typedef struct s_mini
{
	t_token	*token;
	t_ast	*ast;
	char	**env;
	char	**av;
	//int		ac;
	//int		heredoc;
	//???		histoire(readlin add history);
	//struct t_mini *next;
}	t_mini;


//tokenizing
void	init_token(t_mini *mini);

//init_ast
void	init_ast(t_mini *mini);

#endif
