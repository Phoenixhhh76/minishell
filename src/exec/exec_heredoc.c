/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:06:45 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/16 11:15:58 by hho-troc         ###   ########.fr       */
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

int	fork_heredocs(t_mini *mini, t_cmd *cmd, char *delimiter, int i)
{
	pid_t	pid;
	char	*line;
	char	*expanded;
	int		status;
	int		pipefd[2];

	line = NULL;
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		while (1)
		{
			//printf("entered loop heredoc\n");
			line = readline("> ");
			if (g_signal_pid == 1)
			{
			//	printf("if (g_signal_pid == 1)\n");
				safe_exit(mini, 0);
			}
			if (!line || ft_strcmp(line, delimiter) == 0)
				break ;
			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
				safe_exit(mini, 0);
			if (cmd->heredocs_quote[i] == Q_NONE)
			{
				expanded = expand_heredoc_line(line, mini);
				write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
				write(cmd->heredoc_pipe[i][1], "\n", 1);
				free(expanded);
			}
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
		if (line)
			free(line);
		close(pipefd[1]);
		safe_exit(mini, 0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		signal(SIGINT, SIG_DFL);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			g_signal_pid = 1;
			cmd->flag_hd = 1;
			close(pipefd[0]);
			write(STDOUT_FILENO, "\n", 1);
			return (130);
		}
		cmd->heredoc_pipe[i][0] = pipefd[0];
		cmd->heredoc_pipe[i][1] = -1;
	}
	return (0);
}

// int	exec_heredocs(t_cmd *cmd, t_mini *mini) //add t_mini *mini for expand function
// {
// 	int		i;
// 	char	*line;
// 	char	*expanded;

// 	i = 0;
// 	while (i < cmd->heredoc_nb)
// 	{
// 		while (1)
// 		{
// 			// if (g_signal_pid == SIGINT)
// 			// 	return (1);
// 			line = readline("> ");
// 			if (!line || ft_strcmp(line, cmd->heredocs[i]) == 0)
// 				break ;
// 			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
// 				return (-1);//error
// 			if (cmd->heredocs_quote[i] == Q_NONE)
// 			{
// 				expanded = expand_heredoc_line(line, mini);
// 				write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
// 				write(cmd->heredoc_pipe[i][1], "\n", 1);
// 				free(expanded);
// 			}
// 			else
// 			{
// 				write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
// 				write(cmd->heredoc_pipe[i][1], "\n", 1);
// 			}
// 			free(line);
// 		}
// 		free(line);
// 		close(cmd->heredoc_pipe[i][1]);
// 		i++;
// 	}
// 	return (0);
// }

// void	check_heredocs(t_ast *ast, t_mini *mini)// add t_mini *mini
// {
// 	if (!ast)
// 		return ;
// 	// if (g_signal_pid == 1)
// 	// 	return ;
// 	if (ast->ast_token.type == PIPE)
// 	{
// 		check_heredocs(ast->left, mini);
// 		check_heredocs(ast->right, mini);
// 	}
// 	else if (ast->ast_token.type == CMD && ast->cmd && ast->cmd->heredoc_nb > 0)
// 		exec_heredocs(ast->cmd, mini);
// }
