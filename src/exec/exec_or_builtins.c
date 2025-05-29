/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_or_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-29 14:55:09 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-29 14:55:09 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_child(t_mini *mini)
{
	signal(SIGQUIT, SIG_DFL);
	exec_ast(mini, mini->ast, mini->env);
	safe_exit(mini, mini->last_exit);
}

void	exec_single_builtin(t_mini *mini)
{
	int	in_fd;
	int	out_fd;
	int	fd;

	in_fd = dup(STDIN_FILENO);
	out_fd = dup(STDOUT_FILENO);
	if (mini->ast->cmd->in_error != 1 && mini->ast->cmd->path_error != 1)
	{
		if (has_redirection(mini->ast->cmd))
			handle_redirects(mini->ast->cmd);
		if (!ft_strcmp(mini->ast->cmd->cmd_args[0], "exit"))
		{
			mini->cpy_in_fd = in_fd;
			mini->cpy_out_fd = out_fd;
		}
		mini->last_exit = ft_run_builtin(mini, mini->ast->cmd);
		dup2(out_fd, STDOUT_FILENO);
		fd = open("/dev/tty", O_RDONLY);
		if (fd != -1)
			dup2(fd, STDIN_FILENO);
		close(fd);
		// dup2(in_fd, STDIN_FILENO);
		close(out_fd);
		close(in_fd);
	}
}

void	exec_or_builtin(t_mini *mini)
{
	pid_t	pid;
	int		status;

	if (!mini->ast)
		return ;
	if (!is_there_pipe(mini) && ft_builtin(mini->ast))
	{
		exec_single_builtin(mini);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		mini->last_exit = -2;
		return ;
	}
	signal(SIGINT, signal_handler_child);
	if (pid == 0)
		exec_child(mini);
	waitpid(pid, &status, 0);
	signal(SIGINT, signal_handler);
	handle_exit_status(mini, status);
	close_all_heredocs(mini->ast);
}



// void	exec_or_builtin(t_mini *mini)
// {
// 	pid_t	pid;
// 	int		status;
// 	int		out_fd;
// 	int		in_fd;
// 	int		fd;

// 	if (!mini->ast)
// 		return ;
// 	if (!is_there_pipe(mini) && ft_builtin(mini->ast))
// 	{
// 		if (mini->ast->cmd->in_error != 1 && mini->ast->cmd->path_error != 1)
// 		{
// 			in_fd = dup(STDIN_FILENO);
// 			out_fd = dup(STDOUT_FILENO);
// 			if (has_redirection(mini->ast->cmd))
// 				handle_redirects(mini->ast->cmd);
// 			if (!ft_strcmp(mini->ast->cmd->cmd_args[0], "exit"))
// 			{
// 				mini->cpy_in_fd = in_fd;
// 				mini->cpy_out_fd = out_fd;
// 			}
// 			mini->last_exit = ft_run_builtin(mini, mini->ast->cmd);
// 			if (mini->last_exit == -2)
// 				perror("Wrong use of ft_run_builtin");
// 			dup2(out_fd, STDOUT_FILENO);
// 			fd = open("/dev/tty", O_RDONLY);
// 			if (fd != -1)
// 				dup2(fd, STDIN_FILENO);
// 			close(fd);
// 			// dup2(in_fd, STDIN_FILENO);
// 			close(out_fd);
// 			close(in_fd);
// 			return ;
// 		}
// 	}
// 	else
// 	{
// 		pid = fork();
// 		if (pid < 0)
// 		{
// 			perror("fork");
// 			mini->last_exit = -2;
// 		}
// 		signal(SIGINT, signal_handler_child);
// 		if (pid == 0)
// 		{
// 			signal(SIGQUIT, SIG_DFL);
// 			exec_ast(mini, mini->ast, mini->env);
// 			safe_exit(mini, mini->last_exit);
// 		}
// 		waitpid(pid, &status, 0);
// 		signal(SIGINT, signal_handler);
// 		if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == SIGINT)
// 				mini->last_exit = 130;
// 			else if (WTERMSIG(status) == SIGQUIT)
// 				mini->last_exit = 131;
// 		}
// 		else if (WIFEXITED(status))
// 				mini->last_exit = WEXITSTATUS(status);
// 	}
// 	close_all_heredocs(mini->ast);
// }