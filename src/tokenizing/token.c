/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:15:58 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_token_loop_vars(char **current, t_quote *qt)
{
	*current = ft_strdup("");
	if (!*current)
		exit_error("malloc failed in init_token_loop_vars");
	*qt = Q_NONE;
}

/*
this function is used to fill the current token with the input string
it will check if the current character is a space or a meta character
if it is not, it will check if the character is a quote
if it is a quote, it will call the extract_quoted function
if it is not a quote, it will call the extract_plain function
if the current character is a space or a meta character, it will break the loop
*/
void	fill_current_token(const char *input,
				int *i, char **current, t_quote *qt)
{
	while (input[*i] && !ft_isspace(input[*i]) && !is_meta_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			*current = extract_quoted(input, i, *current, qt);
		else
			*current = extract_plain(input, i, *current);
	}
}

void	finalize_token(char *current, t_quote qt, t_token **tokens)
{
	if (current[0] || qt != Q_NONE)
		append_t(tokens, create_t(current, qt));
	else
		free(current);
}

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

t_token	*tokenize_input(const char *input)
{
	t_token			*tokens;
	char			*current;
	t_quote	qt;
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
