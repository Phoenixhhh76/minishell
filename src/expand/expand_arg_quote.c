/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_quote.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:58:15 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/22 21:09:48 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*handle_double_quote(const char *str, int *i, t_mini *mini)
{
	//int		start;
	char	*result;

	//start = ++(*i);
	result = ft_strdup("");
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
			result = ft_strjoin_ff(result, expand_var(str, i, mini));
		else
			result = ft_strjoin_ff(result, ft_strndup(str + (*i)++, 1));
	}
	if (str[*i] == '"')
		(*i)++;
	return (result);
}
