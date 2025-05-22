/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:06:45 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/21 12:23:44 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	fork_heredocs(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
{
	pid_t	pid;
	char	*line;
	//char	*expanded;
	int		status;

	line = NULL;
	cmd->flag_hd = 0;
	g_signal_pid = 0;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		//dprintf(2, "pid pid pid pid pid pid pid pid %d \n\n\n\n\n", getpid());
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		close(cmd->heredoc_pipe[i][0]);
		while (1)
		{
			line = readline("> ");
			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
			{
				safe_exit(mini, 0);
			}
			if (!line)
			{
				if (g_signal_pid == SIGINT)
				{
					safe_cleanup(mini, line);
					safe_exit(mini, 130);
				}
				ft_putstr_fd("minishell: warning: ", 2);
				ft_putstr_fd("here-document delimited by end-of-file ", 2);
				err_msg("(wanted `", delimiter, "')", 0);
				break ;
			}
			if (ft_strcmp(line, delimiter) == 0)
			{
				break ;
			}
			// if (cmd->heredocs_quote[i] == Q_NONE)
			// {
			// 	expanded = expand_heredoc_line(line, mini);
			// 	write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
			// 	write(cmd->heredoc_pipe[i][1], "\n", 1);
			// 	free(expanded);
			// }
			else
			{
				write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
				write(cmd->heredoc_pipe[i][1], "\n", 1);
			}
			free(line);
		}
		if (line)
			free(line);
		close(cmd->heredoc_pipe[i][1]);
		//dprintf(2, "pid pid pid pid pid pid pid pid %d \n\n\n\n\n", getpid());
		free_cmd(cmd);
		//free(cmd);
		safe_exit(mini, 0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(cmd->heredoc_pipe[i][1]);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		if (status / 256 == 130)
		{
			cmd->flag_hd = 1;
			mini->stop_hd = 1;
			mini->last_exit = 130;
			close(cmd->heredoc_pipe[i][0]);
			free(line);
			return (130);
		}
	}
	return (0);
}

// static void	write_heredoc_line(t_cmd *cmd, char *line, char *expanded, int i)
// {
// 	if (cmd->heredocs_quote[i] == Q_NONE)
// 	{
// 		expanded = expand_heredoc_line(line, NULL);
// 		write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
// 		free(expanded);
// 	}
// 	else
// 		write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
// 	write(cmd->heredoc_pipe[i][1], "\n", 1);
// }

// static int	check_line_exit(t_mini *mini, char *line, char *delimiter)
// {
// 	if (!line)
// 	{
// 		if (g_signal_pid == SIGINT)
// 		{
// 			safe_cleanup(mini, line);
// 			safe_exit(mini, 130);
// 		}
// 		ft_putstr_fd("minichell: warning: ", 2);
// 		ft_putstr_fd("here-document delimited by end-of-file ", 2);
// 		err_msg("(wanted `", delimiter, "')", 0);
// 		return (1);
// 	}
// 	return (0);
// }

// int	parent_heredoc(t_mini *mini, t_cmd *cmd, int i, int *status)
// {
// 	signal(SIGINT, SIG_IGN);
// 	close(cmd->heredoc_pipe[i][1]);
// 	//waitpid(pid, &status, 0);
// 	waitpid(-1, status, 0);
// 	signal(SIGINT, SIG_DFL);
// 	if (*status / 256 == 130)
// 	{
// 		cmd->flag_hd = 1;
// 		mini->stop_hd = 1;
// 		mini->last_exit = 130;
// 		close(cmd->heredoc_pipe[i][0]);
// 		return (130);
// 	}
// 	return (0);
// }

// void	child_heredoc(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
// {
// 	char	*line;

// 	signal(SIGINT, heredoc_sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	close(cmd->heredoc_pipe[i][0]);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
// 			safe_exit(mini, 0);
// 		if (check_line_exit(mini, line, delimiter))
// 			break ;
// 		if (ft_strcmp(line, delimiter) == 0)
// 			break ;
// 		write_heredoc_line(cmd, line, NULL, i);
// 		free(line);
// 	}
// 	free(line);
// 	close(cmd->heredoc_pipe[i][1]);
// 	safe_exit(mini, 0);
// }

// int	fork_heredocs(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
// {
// 	pid_t	pid;
// 	int		status;

// 	cmd->flag_hd = 0;
// 	g_signal_pid = 0;
// 	pid = fork();
// 	if (pid == -1)
// 		return (perror("fork"), 1);
// 	if (pid == 0)
// 		child_heredoc(mini, cmd, delimiter, i);
// 	else
// 		parent_heredoc(mini, cmd, i, &status);
// 	return (0);
// }
