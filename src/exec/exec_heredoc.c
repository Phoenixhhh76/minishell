/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 13:06:45 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/09 12:50:25 by hho-troc         ###   ########.fr       */
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
		if (tmp->type == HEREDOC && tmp->next)
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
		return (NULL);
	cmd->heredocs_quote = (t_quote_type *)ft_calloc(nb, sizeof(t_quote_type));
	if (!cmd->heredocs_quote)
		return (NULL);
	while (tmp && tmp != end)
	{
		if (tmp->type == HEREDOC && tmp->next)
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

/* We have to get quote_type of limiter to see if we expand $ in heredoc */
int	exec_heredocs(t_cmd *cmd, t_mini *mini) //add t_mini *mini for expand function
{
	int		i;
	char	*line;
	char	*expanded;

	i = 0;
	while (i < cmd->heredoc_nb)
	{
		while (1)
		{
			line = readline("> ");
			if (g_signal_pid == 1 || !line || ft_strcmp(line, cmd->heredocs[i]) == 0)
				break ;
			if (!cmd->heredoc_pipe || !cmd->heredoc_pipe[i])
				return (-1);//error
			if (cmd->heredocs_quote[i] == QUOTE_NONE) // have to expand a
			{
				expanded = expand_heredoc_line(line, mini);
				//expanded = expand_arg(line, mini, QUOTE_NONE); dosen't work for '$USER'
				write(cmd->heredoc_pipe[i][1], expanded, ft_strlen(expanded));
				write(cmd->heredoc_pipe[i][1], "\n", 1);
				free(expanded);
			}
			else
			{
				write(cmd->heredoc_pipe[i][1], line, ft_strlen(line));
				write(cmd->heredoc_pipe[i][1], "\n", 1);
			}
			free(line);
		}
		free(line);
		close(cmd->heredoc_pipe[i][1]);
		i++;
	}
	//printf("signal_pid= %d\n", g_signal_pid);
	return (0);
}

void	check_heredocs(t_ast *ast, t_mini *mini)// add t_mini *mini
{
	if (!ast)
		return ;
	if (ast->ast_token.type == PIPE)
	{
		check_heredocs(ast->left, mini);
		check_heredocs(ast->right, mini);
	}
	else if (ast->ast_token.type == CMD && ast->cmd && ast->cmd->heredoc_nb > 0)
		exec_heredocs(ast->cmd, mini);
}


