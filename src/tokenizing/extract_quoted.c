/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:52:58 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 14:05:23 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_single_quote(const char *input, int *i)
{
	int		start;
	char	*chunk;

	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	chunk = ft_strndup(&input[start], *i - start);
	if (input[*i] == '\'')
		(*i)++;
	return (chunk);
}

static char	*extract_double_quote(const char *input, int *i, t_mini *mini)
{
	char	*result;
	char	*chunk;

	result = ft_strdup("");
	if (input[*i] == '$' && (input[*i + 1] == '"' || input[*i + 1] == '\0'))
	{
		(*i)++;
		if (input[*i] == '"')
			(*i)++;
		return (ft_strjoin_ff(result, ft_strdup("$")));
	}
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			chunk = expand_var(input, i, mini);
		else
			chunk = ft_strndup(&input[(*i)++], 1);
		result = ft_strjoin_ff(result, chunk);
	}
	if (input[*i] == '"')
		(*i)++;
	return (result);
}

char	*extract_quoted(const char *input, int *i, t_quote *qt, t_mini *mini)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	if (quote == '\'')
	{
		*qt = Q_S;
		return (extract_single_quote(input, i));
	}
	else
	{
		*qt = Q_D;
		return (extract_double_quote(input, i, mini));
	}
}

/*
** Check if current input position is a Bash-style $"..." or $'...'
** echo $"hello" -> hello
*/
bool	is_dollar_quote(const char *input, int i)
{
	return (input[i] == '$' && (input[i + 1] == '"' || input[i + 1] == '\''));
}

bool	has_closing_quote(const char *input, int i, char quote)
{
	i++;
	while (input[i])
	{
		if (input[i] == quote)
			return (true);
		i++;
	}
	return (false);
}
