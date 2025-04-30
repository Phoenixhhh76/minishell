/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-25 13:06:45 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-25 13:06:45 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (tmp->type == HEREDOC && tmp->next)
		{
			tab_heredocs[i] = expand_arg(tmp->next->str, mini);
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
	tab_pipe = (int **)ft_calloc(heredoc_nb, sizeof(int *));
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

int	exec_heredocs(t_cmd *cmd)
{
	int		i;
	char	*line;

	i = 0;
	while (i < cmd->heredoc_nb)
	{
		while (1)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, cmd->heredocs[i]) == 0)
				break ;
			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
				return (-1);//error
			write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
			write(cmd->heredoc_pipe[i][1], "\n", 1);
			free(line);
		}
		free(line);
		close(cmd->heredoc_pipe[i][1]);
		i++;
	}
	return (0);
}

void	check_heredocs(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->ast_token.type == PIPE)
	{
		check_heredocs(ast->left);
		check_heredocs(ast->right);
	}
	else if (ast->ast_token.type == CMD && ast->cmd && ast->cmd->heredoc_nb > 0)
		exec_heredocs(ast->cmd);
}
