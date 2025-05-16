/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:52:58 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:17:13 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_quote_len(const char *input, int start, char quote)
{
	int	i;

	i = start;
	while (input[i] && input[i] != quote)
		i++;
	return (i - start);
}

static void	set_quote_type(char quote, t_quote *qt)
{
	if (quote == '"')
		*qt = Q_D;
	else if (quote == '\'')
		*qt = Q_S;
}

/*
""''echo hola""'''' que""'' tal""''
we skip the first and last quote if in pair
*/
static int	should_strip_quotes(const char *input, int i, const char *current)
{
	return (current[0] == '\0'
		&& (input[i] == '\0' || ft_isspace(input[i])
			|| is_meta_char(input[i])));
}

char	*extract_quoted(const char *input, int *i,
						char *current, t_quote *qt)
{
	char	quote;
	int		start;
	int		len;
	int		strip;
	char	*quoted;

	quote = input[(*i)++];
	start = *i;
	len = get_quote_len(input, start, quote);
	if (input[*i + len] == quote)
		*i += len + 1;
	else
		*i += len;
	strip = should_strip_quotes(input, *i, current);
	if (strip)
	{
		quoted = ft_strndup(&input[start], len);
		set_quote_type(quote, qt);
	}
	else
	{
		quoted = ft_strndup(&input[start - 1], len + 2);
		*qt = Q_NONE;
	}
	return (ft_strjoin_f(current, quoted));
}
