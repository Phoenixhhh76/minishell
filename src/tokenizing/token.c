/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/09 09:17:21 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_token_loop_vars(char **current, t_quote_type *qt)
{
	*current = ft_strdup("");
	if (!*current)
		exit_error("malloc failed in init_token_loop_vars");
	*qt = QUOTE_NONE;
}

void	fill_current_token(const char *input,
				int *i, char **current, t_quote_type *qt)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_meta_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			*current = extract_quoted(input, i, *current, qt);
		else
			*current = extract_plain(input, i, *current);
	}
}

void	finalize_token(char *current, t_quote_type qt, t_token **tokens)
{
	if (current[0] || qt != QUOTE_NONE)
		append_t(tokens, create_t(current, qt));
	else
		free(current);
}

void	check_and_handle_meta(const char *input, int *i, t_token **tokens)
{
	if (is_meta_char(input[*i]))
		*i = handle_meta(input, *i, tokens);
	else if (input[*i])
		(*i)++;
}

t_token	*tokenize_input(const char *input)
{
	t_token			*tokens;
	char			*current;
	t_quote_type	qt;
	int				i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		init_token_loop_vars(&current, &qt);
		fill_current_token(input, &i, &current, &qt);
		finalize_token(current, qt, &tokens);
		check_and_handle_meta(input, &i, &tokens);
	}
	return (tokens);
}
