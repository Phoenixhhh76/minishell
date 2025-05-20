/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast_args_count.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:38:14 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/20 11:49:28 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_token_args(t_token *tok, t_mini *mini)
{
	char	*expanded;
	int		count;

	count = 0;
	if (tok->quote_type == Q_S || tok->quote_type == Q_D)
		return (1);
	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (0);
	if (expanded[0] != '\0' || tok->quote_type != Q_NONE)
		count = count_expanded_split(expanded);
	free(expanded);
	return (count);
}

int	count_expanded_split(char *expanded)
{
	char	**split;
	int		count;
	int		i;

	count = 0;
	i = 0;
	split = ft_split(expanded, ' ');
	while (split && split[i])
	{
		count++;
		i++;
	}
	free_double_tab(split);
	return (count);
}

int	count_export_args(t_token *start, t_token *end, t_mini *mini)
{
	int			count;
	char		*expanded;
	t_token		*tmp;

	count = 0;
	tmp = start;
	while (tmp && tmp != end)
	{
		if (tmp->type == CMD || tmp->type == UNKNOWN)
		{
			expanded = expand_if_needed(tmp, mini);
			if (expanded && (expanded[0] != '\0' || tmp->quote_type != Q_NONE))
				count++;
			free(expanded);
		}
		tmp = tmp->next;
	}
	return (count);
}

void	fill_export_args(char **args,
					t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	char	*expanded;

	i = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			expanded = expand_if_needed(start, mini);
			if (expanded && (expanded[0] != '\0' \
							|| start->quote_type != Q_NONE))
				args[i++] = expanded;
			else
				free(expanded);
		}
		start = start->next;
	}
	args[i] = NULL;
}
