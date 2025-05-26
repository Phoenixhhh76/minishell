/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_quoted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:52:58 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 18:33:41 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_quoted(const char *input, int *i, t_quote *qt)
{
	char	quote ;
	int		start;
	int		len;
	char 	*chunk;

	quote = input[(*i)++];
	start = *i;
	while (input[*i] && input[*i] != quote)
	{
		//printf("[raw] i=%d, char=%c\n", *i, input[*i]);
		(*i)++;
	}
	len = *i - start;
	if (input[*i] == quote)
		(*i)++;
	if (quote == '\'')
		*qt = Q_S;
	else if (quote == '"')
		*qt = Q_D;
	chunk = ft_strndup(&input[start], len);
	//printf("[extract_quoted] -> %.*s\n", len, &input[start]);
	//printf("[DEBUG after quote] i=%d char=%c\n", *i, input[*i]);
	return (chunk);
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