/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:41:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 13:40:05 by hho-troc         ###   ########.fr       */
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

char	*extract_plain(const char *input, int *i, char *current)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && \
		input[*i] != '"' && input[*i] != '\'' && \
		!is_meta_char(input[*i]))
		(*i)++;
	return (ft_strjoin_ff(current, ft_strndup(&input[start], *i - start)));
}

/*
check if the meta character is ||, <<< or >>> : syntax error
if it is not, >> << we append the token to the list
 */
int	handle_meta(const char *input, int i, t_token **tokens)
{
	int	len;

	if ((input[i] == '|' && input[i + 1] == '|') || \
	(input[i] == '<' && input[i + 1] == '<' && input[i + 2] == '<') || \
	(input[i] == '>' && input[i + 1] == '>' && input[i + 2] == '>'))
	{
		syntax_err_msg("unexpected token ", ft_strndup(&input[i], 2), 2);
		return (-1);
	}
	if (input[i] == input[i + 1])
		len = 2;
	else
		len = 1;
	append_t(tokens, create_t(ft_strndup(&input[i], len), Q_NONE));
	return (i + len);
}
