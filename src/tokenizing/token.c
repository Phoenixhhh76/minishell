/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/01 11:17:16 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_and_handle_meta(const char *input, int *i, t_token **tokens)
{
	int		new_i;

	if (is_meta_char(input[*i]))
	{
		new_i = handle_meta(input, *i, tokens);
		if (new_i == -1)
		{
			free_token_list(*tokens);
			*tokens = NULL;
			*i = ft_strlen(input);
			return ;
		}
		*i = new_i;
	}
	else if (input[*i])
		(*i)++;
}

t_token	*tokenize_input(const char *input, t_mini *mini)
{
	t_token			*tokens;
	t_parse_state	ps;

	ps.i = 0;
	ps.glued = false;
	tokens = NULL;
	while (input[ps.i])
	{
		if (ft_isspace(input[ps.i]))
		{
			skip_spaces(input, &ps.i);
			ps.glued = false;
			continue ;
		}
		fill_current_token(input, &ps, &tokens, mini);
		if (is_meta_char(input[ps.i]))
			check_and_handle_meta(input, &ps.i, &tokens);
	}
	return (tokens);
}

/* void	print_token_list(t_token *tokens)
{
	while (tokens)
	{
		printf("Token [%s] type=%d quote=%d glued=%d\n",
			tokens->str, tokens->type, tokens->quote_type, tokens->glued);
		tokens = tokens->next;
	}
} */
