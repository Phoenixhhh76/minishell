/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:39:45 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/15 15:34:13 by hho-troc         ###   ########.fr       */
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

typedef enum e_node_type {
    NODE_COMMAND,
    NODE_PIPE,
    NODE_REDIR_OUT,
    NODE_REDIR_IN,
    NODE_REDIR_APPEND,
    NODE_HEREDOC
}   t_node_type;

typedef struct s_ast {
    t_node_type     type;
    char            **cmd_args;     // 如果是 command 就存 argv
    int	fd[2];      // 如果是 redir 就存檔名
	char	*cmd_path;
    struct s_ast    *left;          // 左子樹（前一個語法單元）
    struct s_ast    *right;         // 右子樹（下一個語法單元）
}   t_ast;


typedef struct s_mini
{
	t_ast	ast;
	char	**env;
	char	**av;
	int		ac;
	int		heredoc;
	//???		histoire(readlin add history);
	//int		fd_in;
	//int		fd_out;
	//int		*pipe;
	//int		nb_cmds;
	//int		child;
	//int		*pids;
	//char	**cmd_opts;
	char	*cmd_path;
	//struct t_mini *next;
}	t_mini;


#endif