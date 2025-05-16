/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 15:18:34 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_single(char **args, int i, t_token *tok)
{
	args[i++] = ft_strdup(tok->str);
	return (i);
}

int	add_split(char **args, int i, char *expanded)
{
	char	**split;
	int		j;

	split = ft_split(expanded, ' ');
	j = 0;
	while (split && split[j])
		args[i++] = ft_strdup(split[j++]);
	free(expanded);
	free_double_tab(split);
	return (i);
}
/*
the condition strchr ", make sure if we have a string like
$HOLA="  bonjour he ",have space at the beginning and end
Token [wesh"$HOLA"]  quote_type 0, we won't split it, we keep "  bonjour he "
 */
int	handle_expanded(char **args, int i, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (i);
	if (tok->quote_type == Q_D || ft_strchr(tok->str, '"'))
	{
		args[i++] = expanded;
	}
	else
	{
		i = add_split(args, i, expanded);
	}
	return (i);
}

int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	if (tok->quote_type == Q_S)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}
/*
static int	should_count_or_add(t_token *tok, t_mini *mini, char ***args, int *i)
{
	char	*expanded;

	if (tok->quote_type == Q_S || tok->quote_type == Q_D)
	{
		if (args)
			(*args)[(*i)++] = ft_strdup(tok->str);
		return (1);
	}
	expanded = expand_if_needed(tok, mini);
	if (!expanded || (expanded[0] == '\0' && tok->quote_type == Q_NONE))
		return (free(expanded), 0);
	if (args)
		(*args)[(*i)++] = expanded;
	else
		free(expanded);
	return (1);
} */

int	count_expanded_split(char *expanded)
{
	char	**split;
	int		count = 0;
	int		i = 0;

	split = ft_split(expanded, ' ');
	while (split && split[i])
	{
		count++;
		i++;
	}
	free_double_tab(split);
	return (count);
}

int	count_token_args(t_token *tok, t_mini *mini)
{
	char	*expanded;
	int		count = 0;

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

int	count_args_advanced(t_token *start, t_token *end, t_mini *mini)
{
	int	count = 0;

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

char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		i;
	int		size;
	char	**args;

	size = count_args_advanced(start, end, mini);
	args = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			i = process_token(args, i, start, mini);
		else if ((start->type == R_IN || start->type == R_OUT || \
			start->type == R_A || start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

char	**collect_args_for_export(t_token *start, t_token *end, t_mini *mini)
{
	int		count;
	int		i;
	char	*expanded;
	char	**args;
	t_token	*tmp;

	count = 0;
	i = 0;
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
	args = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
		{
			expanded = expand_if_needed(start, mini);
			if (expanded && (expanded[0] != '\0' || start->quote_type != Q_NONE))
				args[i++] = expanded;
			else
				free(expanded);
		}
		start = start->next;
	}
	args[i] = NULL;
	return (args);
}

void	init_ast(t_mini *mini)
{
	mini->ast = parse_pipeline(mini->token, NULL, mini);
}
