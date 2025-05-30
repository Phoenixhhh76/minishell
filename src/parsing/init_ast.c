/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 15:15:24 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	process_split_argument(char ***args, int *i, char *arg)
{
	char	**split;
	int		j;

	split = ft_split(arg, ' ');
	free(arg);
	if (!split || !split[0])
	{
		(*args)[(*i)++] = ft_strdup("");
		free_double_tab(split);
		return ;
	}
	j = 0;
	while (split[j])
	{
		(*args)[(*i)++] = ft_strdup(split[j]);
		j++;
	}
	free_double_tab(split);
}

static bool	should_preserve_empty_arg(t_token *from, t_token *to)
{
	while (from && from != to)
	{
		if (from->quote_type != Q_NONE && from->str && from->str[0] != '\0')
			return (true);
		from = from->next;
	}
	return (false);
}

void	process_argument_token(char ***args,
				int *i, t_token **start_ptr, t_mini *mini)
{
	t_token	*prev;
	t_token	*scan;
	bool	quoted;
	char	*arg;

	prev = *start_ptr;
	scan = *start_ptr;
	arg = join_tokens_for_arg(start_ptr, mini, false);
	if (*start_ptr == prev)
		*start_ptr = prev->next;
	quoted = should_preserve_empty_arg(scan, *start_ptr);
	if (quoted)
	{
		if (arg && arg[0])
			(*args)[(*i)++] = arg;
		else
		{
			(*args)[(*i)++] = ft_strdup("");
			free(arg);
		}
	}
	else
		process_split_argument(args, i, arg);
}

bool	handle_quoted_empty_token(char **arg_slot, t_token **start_ptr)
{
	t_token	*start;

	start = *start_ptr;
	if (start->quote_type != Q_NONE && (!start->str || start->str[0] == '\0'))
	{
		*arg_slot = ft_strdup("");
		*start_ptr = start->next;
		return (true);
	}
	return (false);
}
