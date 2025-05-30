/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:38:14 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 12:02:23 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_token_args(t_token *tok, t_mini *mini)
{
	char	*expanded;
	int		count;

	count = 0;
	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (0);
	if (tok->quote_type == Q_S || tok->quote_type == Q_D)
		count = 1;
	else if (expanded[0] != '\0')
		count = count_expanded_split(expanded);
	else
		count = 0;
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

int	count_args_advanced(t_token *start, t_token *end, t_mini *mini)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			count += count_token_args(start, mini);
		else if ((start->type == R_IN || start->type == R_OUT || \
				start->type == R_A || start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	return (count);
}
