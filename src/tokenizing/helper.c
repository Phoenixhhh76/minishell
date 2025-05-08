/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:41:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/08 21:00:01 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_meta_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(const char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}
/* too long, seperate in extract_quote.c */
// char	*extract_quoted(const char *input, int *i, char *current, t_quote_type *qt)
// {
// 	char quote = input[(*i)++];
// 	int start = *i;
// 	int is_wrapped = (current[0] == '\0');
// 	while (input[*i] && input[*i] != quote)
// 		(*i)++;
// 	int len = *i - start;
// 	if (input[*i] == quote)
// 		(*i)++;
// 	char *quoted;
// 	if (is_wrapped && (input[*i] == '\0' || ft_isspace(input[*i]) || is_meta_char(input[*i])))
// 	{
// 		quoted = ft_strndup(&input[start], len);
// 		*qt = (quote == '"') ? QUOTE_DOUBLE : QUOTE_SINGLE;
// 	}
// 	else
// 	{
// 		quoted = ft_strndup(&input[start - 1], len + 2);
// 		*qt = QUOTE_NONE;
// 	}
// 	return ft_strjoin_f(current, quoted);
// }

char	*extract_plain(const char *input, int *i, char *current)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && \
		input[*i] != '"' && input[*i] != '\'' && \
		!is_meta_char(input[*i]))
		(*i)++;
	return (ft_strjoin_f(current, ft_strndup(&input[start], *i - start)));
}

int	handle_meta(const char *input, int i, t_token **tokens)
{
	int len = (input[i] == input[i + 1]) ? 2 : 1;
	append_t(tokens, create_t(ft_strndup(&input[i], len), QUOTE_NONE));
	return (i + len);
}
