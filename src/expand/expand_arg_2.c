/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_arg_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:20:31 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 15:23:07 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_char(const char *str, int *i,
											t_mini *mini, t_quote quote_type)
{
	if (str[*i] == '\'' && quote_type == Q_NONE)
		return (handle_single_quote(str, i));
	else if (str[*i] == '"')
		return (handle_double_quote(str, i, mini));
	else if (str[*i] == '$')
	{
		if (!str[*i + 1] || !(ft_isalpha(str[*i + 1]) || \
								str[*i + 1] == '_' || str[*i + 1] == '?'))
		{
			(*i)++;
			return (ft_strdup("$"));
		}
		else
			return (expand_var(str, i, mini));
	}
	else
		return (ft_strndup(&str[(*i)++], 1));
}

static char	*expand_arg_loop(const char *str, t_mini *mini, t_quote quote_type)
{
	char	*result;
	char	*chunk;
	int		i;
	int		old_i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		old_i = i;
		chunk = expand_char(str, &i, mini, quote_type);
		if (!chunk)
			break ;
		result = ft_strjoin_ff(result, chunk);
		if (i == old_i)
			i++;
	}
	return (result);
}

char	*expand_arg(const char *str, t_mini *mini,
						t_quote quote_type, bool is_dollar_quote)
{
	if (is_dollar_quote)
		return (ft_strdup(str));
	if (str[0] == '"' && str[1] == '$' && str[2] == '"' && str[3] == '"')
		return (handle_special_case(str));
	return (expand_arg_loop(str, mini, quote_type));
}
