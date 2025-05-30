/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 12:03:14 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	process_argument_token(char ***args,
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

static bool	handle_quoted_empty_token(char **arg_slot, t_token **start_ptr)
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

char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		size;
	char	**args;
	int		i;

	i = 0;
	size = count_args_advanced(start, end, mini);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (handle_quoted_empty_token(&args[i], &start))
		{
			i++;
			continue ;
		}
		if (start->type == CMD || start->type == UNKNOWN)
			process_argument_token(&args, &i, &start, mini);
		else if (is_redirection(start->type) && start->next)
			start = start->next->next;
		else
			start = start->next;
	}
	args[i] = NULL;
	return (args);
}
