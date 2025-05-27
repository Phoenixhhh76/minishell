/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:41:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 18:37:15 by hho-troc         ###   ########.fr       */
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

char	*extract_plain(const char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && \
			input[*i] != '"' && input[*i] != '\'' && \
			input[*i] != '$' && !is_meta_char(input[*i]))
	{
		(*i)++;
	}
	return (ft_strndup(&input[start], *i - start));
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



// char	*ft_strjoin_b(const char *s1, char *s2)
// {
// 	char	*joined;
// 	size_t	len1;
// 	size_t	len2;

// 	if (!s1 && !s2)
// 		return (NULL);
// 	if (!s1)
// 		len1 = 0;
// 	else
// 		len1 = ft_strlen(s1);
// 	if (!s2)
// 		len2 = 0;
// 	else
// 		len2 = ft_strlen(s2);
// 	joined = malloc(len1 + len2 + 1);
// 	if (!joined)
// 		return (NULL);
// 	if (s1)
// 		ft_memcpy(joined, s1, len1);
// 	if (s2)
// 		ft_memcpy(joined + len1, s2, len2);

// 	joined[len1 + len2] = '\0';
// 	if (s2)
// 		free(s2);
// 	return (joined);
// }
