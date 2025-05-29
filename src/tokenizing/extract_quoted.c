/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:52:58 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/29 12:55:19 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*extract_quoted(const char *input, int *i, t_quote *qt)
// {
// 	char	quote ;
// 	int		start;
// 	int		len;
// 	char	*chunk;

// 	quote = input[(*i)++];
// 	start = *i;
// 	while (input[*i] && input[*i] != quote)
// 	{
// 		//printf("[raw] i=%d, char=%c\n", *i, input[*i]);
// 		(*i)++;
// 	}
// 	len = *i - start;
// 	if (input[*i] == quote)
// 		(*i)++;
// 	if (quote == '\'')
// 		*qt = Q_S;
// 	else if (quote == '"')
// 		*qt = Q_D;
// 	chunk = ft_strndup(&input[start], len);
// 	//printf("[extract_quoted] -> %.*s\n", len, &input[start]);
// 	//printf("[DEBUG after quote] i=%d char=%c\n", *i, input[*i]);
// 	return (chunk);
// }

char	*extract_quoted(const char *input, int *i, t_quote *qt, t_mini *mini)
{
	char	quote;
	char	*result;
	char	*chunk;

	quote = input[(*i)++];
	*qt = (quote == '\'') ? Q_S : Q_D;
	result = ft_strdup("");

	if (quote == '\'')
	{
		int start = *i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		chunk = ft_strndup(&input[start], *i - start);
		if (input[*i] == quote)
			(*i)++;
		return (chunk);
	}

	// Handle double quotes: allow variable expansion inside
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



// /*
// ""''echo hola""'''' que""'' tal""''
// we skip the first and last quote if in pair
// */
// static int	should_strip_quotes(const char *input, int i, const char *current)
// {
// 	return (current[0] == '\0'
// 		&& (input[i] == '\0' || ft_isspace(input[i])
// 			|| is_meta_char(input[i])));
// }