/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 20:06:06 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 10:25:06 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_pipe_node(t_mini *mini, t_ast *node, char **envp)
{
	int		status;
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(node->fd) == -1)
		exit_error("pipe");
	left_pid = fork();
	if (left_pid < 0)
	{
		perror("left_pid fork");
		mini->last_exit = 1;
	}
	if (left_pid == 0)
	{
		close(node->fd[0]);
		dup2(node->fd[1], STDOUT_FILENO);
		close(node->fd[1]);
		exec_ast(mini, node->left, envp);
		safe_exit(mini, mini->last_exit);//Nina
	}
	right_pid = fork();
	if (right_pid < 0)
	{
		perror("right_pid fork");
		mini->last_exit = 1;
	}
	if (right_pid == 0)
	{
		close(node->fd[1]);
		dup2(node->fd[0], STDIN_FILENO);
		close(node->fd[0]);
		exec_ast(mini, node->right, envp);
		safe_exit(mini, mini->last_exit);//Nina
	}
	close(node->fd[0]);
	close(node->fd[1]);
	waitpid(right_pid, &status, 0);
	waitpid(left_pid, NULL, 0);
	safe_exit(mini, status / 256);
}

void	handle_redirects(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	if (cmd->infile && cmd->last_redirin == 0)
	{
		fd = open(cmd->infile, O_RDONLY);
		if (fd < 0)
			perror(cmd->infile);
			//exit_error(cmd->infile);
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (cmd->heredoc_nb > 0 && !cmd->flag_hd)
	{
		dup2(cmd->heredoc_pipe[cmd->heredoc_nb - 1][0], STDIN_FILENO);
		while (i < cmd->heredoc_nb)
		{
			if (cmd->heredoc_pipe[i][0] > 0)
				close(cmd->heredoc_pipe[i][0]);
			if (cmd->heredoc_pipe[i][1] > 0)
				close(cmd->heredoc_pipe[i][1]);
			i++;
		}
	}
	if (cmd->outfile)
	{
		if (cmd->append && ft_strcmp(cmd->outfile, cmd->append) == 0)
		{
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		else
		{
			fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open outfile");
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	exec_cmd_node(t_mini *mini, t_ast *node, char **envp)
{
	if (!node->cmd)
		safe_exit(mini, mini->last_exit);
	if (node->cmd->in_error == 1 || node->cmd->path_error == 1)
		safe_exit(mini, 1);
	handle_redirects(node->cmd);
	if ((!node->cmd->append && !node->cmd->heredoc_nb && !node->cmd->infile \
			&& !node->cmd->outfile) && (!node->cmd->cmd_args \
			|| !node->cmd->cmd_args[0] || node->cmd->cmd_args[0][0] == '\0'))
	{
		mini->last_exit = err_msg("", ":", " command not found", 127);
		node->cmd->path_error = 1;
	}
	if (node->cmd->cmd_args && node->cmd->cmd_path)
	{
		if (!node->cmd->cmd_args || !node->cmd->cmd_args[0] \
			|| node->cmd->cmd_args[0][0] == '\0')
		{
			mini->last_exit = err_msg("", ":", " command not found", 127);
			node->cmd->path_error = 1;
		}
		if (access(node->cmd->cmd_path, F_OK) != 0)
		{
			printf(" DEBUG 2 = %i\n", mini->last_exit);
			err_msg(node->cmd->cmd_path, ":", "  command not found", 127);
			safe_exit(mini, 127);
		}
		if (access(node->cmd->cmd_path, X_OK) != 0)
		{
			err_msg(node->cmd->cmd_path, ":", " Permission denied", 126);
			safe_exit(mini, 126);
		}
		execve(node->cmd->cmd_path, node->cmd->cmd_args, envp);
		if (access(node->cmd->cmd_path, X_OK) == 0)
		{
			perror(node->cmd->cmd_args[0]);
			safe_exit(mini, 126);
		}
		mini->last_exit = err_msg(node->cmd->cmd_args[0], ":", \
			" command not found", 127);
	}
	safe_exit(mini, mini->last_exit);
}

void	exec_ast(t_mini *mini, t_ast *node, char **envp)
{
	if (!node)
		return ;
	if (node->ast_token.type == PIPE)
		exec_pipe_node(mini, node, envp);
	else if (node->ast_token.type == CMD)
	{
		// if (g_signal_pid == 2 || node->cmd->flag_hd)
		// {
		// 	mini->last_exit = 130;
		// 	return ;
		// }
		// if (node->cmd->path_error)
			// return ;
		if (ft_builtin(node))
			ft_run_builtin(mini, node->cmd);
		else
			exec_cmd_node(mini, node, envp);
	}
}
