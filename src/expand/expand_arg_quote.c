/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:58:15 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/29 14:54:46 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_double_quote(const char *str, int *i, t_mini *mini)
{
	char	*result;
	char	*chunk;

	result = ft_strdup("");
	(*i)++;
	while (str[*i])
	{
		if (str[*i] == '"')
			break ;
		if (str[*i] == '$')
			chunk = expand_var(str, i, mini);
		else
			chunk = ft_strndup(str + (*i)++, 1);
		result = ft_strjoin_ff(result, chunk);
	}
	if (str[*i] == '"')
		(*i)++;
	return (result);
}

char	*handle_single_quote(const char *str, int *i)
{
	int		start;
	char	*segment;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	segment = ft_strndup(str + start, *i - start);
	if (str[*i] == '\'')
		(*i)++;
	return (segment);
}
