/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-22 13:50:33 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-22 13:50:33 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_heredoc(int nb, t_token *start, t_token *end, t_cmd *cmd)
{
	int		i;
	char	**tab_heredocs;
	t_token	*tmp;

	i = 0;
	tmp = start;
	tab_heredocs = (char **)ft_calloc(nb + 1, sizeof(char *));
	if (!tab_heredocs)
		return (NULL);
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
	tab_pipe = (int **)ft_calloc(heredoc_nb, sizeof(int *));
	if (!tab_pipe)
		return (NULL);
	while (i < heredoc_nb)
	{
		tab_pipe[i] = (int *)ft_calloc(2, sizeof(int));
		if (!tab_pipe[i])
		{
			ft_free_tab_int(tab_pipe, i);
			perror("Heredoc : calloc error");
			return (NULL);
		}
		if (pipe(tab_pipe[i]) == -1)
		{
			ft_free_tab_int(tab_pipe, i);
			perror("Heredoc : pipe error");
			return (NULL);
		}
		i++;
	}
	return (tab_pipe);
}

void	close_all_heredocs(t_ast *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->ast_tok.type == PIPE)
	{
		close_all_heredocs(ast->left);
		close_all_heredocs(ast->right);
	}
	else if (ast->ast_tok.type == CMD && ast->cmd && ast->cmd->heredoc_nb > 0)
	{
		i = 0;
		while (i < ast->cmd->heredoc_nb)
		{
			if (ast->cmd->heredoc_pipe[i][0] > 0)
				close(ast->cmd->heredoc_pipe[i][0]);
			if (ast->cmd->heredoc_pipe[i][1] > 0)
				close(ast->cmd->heredoc_pipe[i][1]);
			i++;
		}
	}
}

void	close_all_heredoc_pipes(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->heredoc_nb)
	{
		if (cmd->heredoc_pipe[i][0] > 0)
			close(cmd->heredoc_pipe[i][0]);
		if (cmd->heredoc_pipe[i][1] > 0)
			close(cmd->heredoc_pipe[i][1]);
		i++;
	}
}
