/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/15 17:05:48 by hho-troc         ###   ########.fr       */
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
//no good for echo wesh"$HOLA" | cat -e  if hola= "  b c " but we keep it for now
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
/* minishell> echo wesh"$USER" | cat -e
Token [echo]  quote_type 0
Token [wesh"$USER"]  quote_type 0
Token [|]  quote_type 0
Token [cat]  quote_type 0
Token [-e]  quote_type 0
weshhho-troc$  -----> ok, because $USER have no space at
the beginning and end

minishell> echo wesh"$HOLA" | cat -e
Token [echo]  quote_type 0
Token [wesh"$HOLA"]  quote_type 0
Token [|]  quote_type 0
Token [cat]  quote_type 0
Token [-e]  quote_type 0
wesh bonjour he$  -----> KO, because $HOLA= " bonjour he "
in real bash
bash-5.1$ export HOLA=" bonjour he "
bash-5.1$ echo wesh"$HOLA" | cat -e
wesh  bonjour    he   $

 */

/*
this function is used to fill the current token with the input string
it will check if the current character is a space or a meta character
if it is not, it will check if the character is a quote
if it is a quote, it will call the extract_quoted function
if it is not a quote, it will call the extract_plain function
if the current character is a space or a meta character, it will break the loop
*/
/* void	fill_current_token(const char *input,
				int *i, char **current, t_quote_type *qt)
{
	t_quote_type	local_qt;

	while (input[*i] && !ft_isspace(input[*i]) && !is_meta_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			local_qt = QUOTE_NONE;
			*current = extract_quoted(input, i, *current, &local_qt);

			if (local_qt == QUOTE_DOUBLE)
				*qt = QUOTE_DOUBLE; // 優先保留雙引號（保留空格用）
			else if (local_qt == QUOTE_SINGLE && *qt == QUOTE_NONE)
				*qt = QUOTE_SINGLE; // 只有在沒有其他 quote 才設定單引號
		}
		else
			*current = extract_plain(input, i, *current);
	}
} */


void	finalize_token(char *current, t_quote_type qt, t_token **tokens)
{
	if (current[0] || qt != QUOTE_NONE)
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
