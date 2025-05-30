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

static void	handle_heredoc_interrupt(t_mini *mini, t_cmd *cmd)
{
	free_cmd(cmd);
	safe_exit(mini, 130);
}

static void	write_heredoc_line(t_cmd *cmd, char *line, t_mini *mini, int i)
{
	char	*expanded;

	if (cmd->heredocs_quote[i] == Q_NONE)
	{
		expanded = expand_heredoc_line(line, mini);
		write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
		write(cmd->heredoc_pipe[i][1], "\n", 1);
		free(expanded);
	}
	else
	{
		write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
		write(cmd->heredoc_pipe[i][1], "\n", 1);
	}
}

static void	child_heredoc_loop(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
			safe_exit(mini, 0);
		if (!line)
		{
			if (g_signal_pid == SIGINT)
				handle_heredoc_interrupt(mini, cmd);
			ft_putstr_fd("minishell: warning: ", 2);
			ft_putstr_fd("here-document delimited by end-of-file ", 2);
			err_msg("(wanted `", delimiter, "')", 0);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
			break ;
		write_heredoc_line(cmd, line, mini, i);
		free(line);
	}
	if (line)
		free(line);
}

static int	heredoc_child_process(t_mini *mini, t_cmd *cmd, char *delim, int i)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(cmd->heredoc_pipe[i][0]);
	child_heredoc_loop(mini, cmd, delim, i);
	close(cmd->heredoc_pipe[i][1]);
	free_cmd(cmd);
	safe_exit(mini, 0);
	return (0);
}

int	fork_heredocs(t_mini *mini, t_cmd *cmd, char *delim, int i)
{
	pid_t	pid;
	int		status;

	cmd->flag_hd = ((g_signal_pid = 0));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -2);
	if (pid == 0)
		heredoc_child_process(mini, cmd, delim, i);
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
			return (130);
		}
	}
	return (0);
}
