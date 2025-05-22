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

/*
char	**get_heredoc(int nb, t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	char	**tab_heredocs;
	t_token	*tmp;

	i = 0;
	tmp = start;
	tab_heredocs = (char **)ft_calloc(nb + 1, sizeof(char *));
	if (!tab_heredocs)
		return (NULL); //error calloc
	while (tmp && tmp != end)
	{
		if (tmp->type == HD && tmp->next)
		{
			//tab_heredocs[i] = expand_arg(tmp->next->str, mini, tmp->next->quote_type);
			//Limiter we don't need to expand
			tab_heredocs[i] = ft_strdup(tmp->next->str);
			tmp = tmp->next;
			i++;
		}
		tmp = tmp->next;
	}
	tab_heredocs[i] = NULL;
	return (tab_heredocs);
} */
/* have to keep quote type, but we can have just 4 argument, so I get nb and quote from CMD */
char	**get_heredoc(int nb, t_token *start, t_token *end, t_cmd *cmd)
{
	int		i;
	char	**tab_heredocs;
	t_token	*tmp;

	i = 0;
	tmp = start;
	tab_heredocs = (char **)ft_calloc(nb + 1, sizeof(char *));
	if (!tab_heredocs)
		return (NULL);//calloc_error
	cmd->heredocs_quote = (t_quote *)ft_calloc(nb, sizeof(t_quote));
	if (!cmd->heredocs_quote)
		return (NULL);
	while (tmp && tmp != end)
	{
		if (tmp->type == HD && tmp->next)
		{
			tab_heredocs[i] = ft_strdup(tmp->next->str);
			cmd->heredocs_quote[i] = tmp->next->quote_type;
			tmp = tmp->next;
			i++;
		}
		tmp = tmp->next;
	}
	tab_heredocs[i] = NULL;
	return (tab_heredocs);
}

int	**create_heredoc_pipe(int heredoc_nb)
{
	int	i;
	int	**tab_pipe;

	i = 0;
	tab_pipe = (int **)ft_calloc(heredoc_nb, sizeof(int [2]));
	if (!tab_pipe)
		return (NULL); //error calloc
	while (i < heredoc_nb)
	{
		tab_pipe[i] = (int *)ft_calloc(2, sizeof(int));
		if (!tab_pipe[i])
			return (NULL); //error calloc
		if (pipe(tab_pipe[i]) == -1)
			return (NULL); //pipe error
		i++;
	}
	return (tab_pipe);
}

void	close_all_heredocs(t_ast *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->ast_token.type == PIPE)
	{
		close_all_heredocs(ast->left);
		close_all_heredocs(ast->right);
	}
	else if (ast->ast_token.type == CMD && ast->cmd && ast->cmd->heredoc_nb > 0)
	{
		i = 0;
		while (i < ast->cmd->heredoc_nb)
		{
			close(ast->cmd->heredoc_pipe[i][0]);
			i++;
		}
	}
}

t_mini	*static_struct(t_mini *mini)
{
	static t_mini	*tmp = NULL;

	if (mini)
		tmp = mini;
	return (tmp);
}

int	fork_heredocs(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
{
	pid_t	pid;
	char	*line;
	char	*expanded;
	int		status;

	line = NULL;
	cmd->flag_hd = 0;
	g_signal_pid = 0;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, heredoc_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		close(cmd->heredoc_pipe[i][0]);
		while (1)
		{
			line = readline("> ");
			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
			{
				dprintf(2, "    !! no heredoc_pipe !!\n");
				safe_exit(mini, 0);
			}
			if (!line)
			{
				//dprintf(2, "  g_signal = %i\n", g_signal_pid);
				if (g_signal_pid == SIGINT)
				{
					dprintf(2, "    // no line ctr+C //\n");
					safe_cleanup(mini, line);
					safe_exit(mini, 130);
				}
				ft_putstr_fd("minichell: warning: ", 2);
				ft_putstr_fd("here-document delimited by end-of-file ", 2);
				err_msg("(wanted `", delimiter, "')", 0);
				break ;
			}
			if (ft_strcmp(line, delimiter) == 0)
			{
				dprintf(2, "    ** found delimiter **\n");
				break ;
			}
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
			dprintf(2, "     >> free line before returning <<\n");
			free(line);
		}
		if (line)
			free(line);
		close(cmd->heredoc_pipe[i][1]);
		safe_exit(mini, 0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(cmd->heredoc_pipe[i][1]);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		if (status / 256 == 130/*WIFSIGNALED(status) && WTERMSIG(status) == SIGINT*/)
		{
			cmd->flag_hd = 1;
			mini->stop_hd = 1;
			mini->last_exit = 130;
			close(cmd->heredoc_pipe[i][0]);
			free(line);
			free(delimiter);
			return (130);
		}
	}
	return (0);
}


// static void	handle_heredoc_line(t_mini *mini, t_cmd *cmd, char *line, int i)
// {
// 	char	*expanded;

// 	if (cmd->heredocs_quote[i] == Q_NONE)
// 	{
// 		expanded = expand_heredoc_line(line, mini);
// 		write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
// 		write(cmd->heredoc_pipe[i][1], "\n", 1);
// 		free(expanded);
// 	}
// 	else
// 	{
// 		write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
// 		write(cmd->heredoc_pipe[i][1], "\n", 1);
// 	}
// }

// static void	print_ctrld(char *delimiter)
// {
// 	ft_putstr_fd("minichell: warning: ", 2);
// 	ft_putstr_fd("here-document delimited by end-of-file ", 2);
// 	err_msg("(wanted `", delimiter, "')", 0);
// }

// static void	child_heredoc(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
// {
// 	char	*line;

// 	signal(SIGINT, heredoc_sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	close(cmd->heredoc_pipe[i][0]);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (cmd->flag_hd == 1 || g_signal_pid == 2)
// 		{
// 			return (free(line);
// 			safe_exit(mini, 130));
// 		}
// 		if (!line)
// 		{
// 			print_ctrld(delimiter);
// 			break ;
// 		}
// 		if (ft_strcmp(line, delimiter) == 0)
// 			break ;
// 		if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
// 			safe_exit(mini, 0);
// 		handle_heredoc_line(mini, cmd, line, i);
// 		free(line);
// 	}
// 	free(line);
// 	close(cmd->heredoc_pipe[i][1]);
// 	safe_exit(mini, 0);
// }

// static int	parent_heredoc(t_mini *mini, t_cmd *cmd, int *status, int i)
// {
// 	signal(SIGINT, SIG_IGN);
// 	close(cmd->heredoc_pipe[i][1]);
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
// 	return (parent_heredoc(mini, cmd, &status, i));
// }
