/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:06:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/23 17:05:29 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* void	exec_ast(t_ast *node, char **envp)
{
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node)
		return ;
	if (node->ast_token.type == PIPE)//PIPE situation
	{
		if (pipe(node->fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		left_pid = fork();
		if (left_pid == 0) //child do left side, write pipe
		{
			close(node->fd[0]);
			dup2(node->fd[1], STDOUT_FILENO);
			close(node->fd[1]);
			exec_ast(node->left, envp);
			exit(1); //error handel have to check
		}
		right_pid = fork();
		if (right_pid == 0) //child do right side, read from pipe
		{
			close(node->fd[1]);
			dup2(node->fd[0], STDIN_FILENO);
			close(node->fd[0]);
			exec_ast(node->right, envp);
			exit(1);//error handel have to check
		}
		// for parent
		close(node->fd[0]);
		close(node->fd[1]);
		waitpid(left_pid, NULL, 0);
		waitpid(right_pid, NULL, 0);
	}
	// ──────────need seperation Pipe and CMD for fit in 25 lines─────────────────────
	else if (node->ast_token.type == CMD && node->cmd)//CMD situation
	{
		if (node->cmd->infile)// input redirection
		{
			int fd = open(node->cmd->infile, O_RDONLY);
			if (fd < 0)
			{
				perror("open infile");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (node->cmd->outfile)// output redirection
		{
			int fd = open(node->cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("open outfile");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		// 處理 builtin
		// if (ft_is_builtin(node->cmd->cmd_args[0]))
		// {
		// 	ft_run_builtin(node->cmd, &envp); // no need path ni fork
		// 	exit(0);
		// }
		// 外部指令
		execve(node->cmd->cmd_path, node->cmd->cmd_args, envp);
		perror("execve");
		exit(1);
	}
} */

void	exec_pipe_node(t_ast *node, char **envp)
{
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(node->fd) == -1)
		exit_error("pipe");
	left_pid = fork();
	if (left_pid == 0)
	{
		close(node->fd[0]);
		dup2(node->fd[1], STDOUT_FILENO);
		close(node->fd[1]);
		exec_ast(node->left, envp);
		exit(1);
	}
	right_pid = fork();
	if (right_pid == 0)
	{
		close(node->fd[1]);
		dup2(node->fd[0], STDIN_FILENO);
		close(node->fd[0]);
		exec_ast(node->right, envp);
		exit(1);
	}
	close(node->fd[0]);
	close(node->fd[1]);
	waitpid(left_pid, NULL, 0);
	waitpid(right_pid, NULL, 0);
}

void	handle_redirects(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	if (cmd->heredoc_nb > 0)
	{
		dup2(cmd->heredoc_pipe[cmd->heredoc_nb - 1][0], STDIN_FILENO);
		while (i < cmd->heredoc_nb)
		{
			close(cmd->heredoc_pipe[i][0]);
			close(cmd->heredoc_pipe[i][1]);
			i++;
		}
	}
	else if (cmd->infile)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			exit_error("open infile");
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->outfile)
	{
		if (ft_strcmp(cmd->outfile, cmd->append) == 0)
		{
			printf("last append detected \n");
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			exit_error("open outfile");
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	exec_cmd_node(t_ast *node, char **envp)
{
	int	i;

	i = 0;
	if (!node->cmd)
		return ;
	handle_redirects(node->cmd);
	// if (ft_is_builtin(node->cmd->cmd_args[0]))
	// {
	// 	ft_run_builtin(node->cmd, &envp);
	// 	exit(0);
	// }
	if (!node->cmd->cmd_path)
	{
		perror("command not found");
		exit(127);//to be determined
	}
	if (node->cmd->cmd_args)
	{
		execve(node->cmd->cmd_path, node->cmd->cmd_args, envp);
		perror("execve");
		exit(1);
	}
}

void	exec_ast(t_ast *node, char **envp)
{
	if (!node)
		return ;
	if (node->ast_token.type == PIPE)
		exec_pipe_node(node, envp);
	else if (node->ast_token.type == CMD)
		exec_cmd_node(node, envp);
}
