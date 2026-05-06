/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:13:08 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/03 10:39:39 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handle a Bash-style $"..." or $'...' string (no variable expansion)
** Extracts the quoted string as-is and sets quote_type.
*/
char	*handle_dollar_quote(const char *input, int *i, t_quote *qt)
{
	char	quote;
	int		start;
	int		len;
	char	*chunk;

	quote = input[*i + 1];
	if (quote == '\'')
		*qt = Q_S;
	else if (quote == '"')
		*qt = Q_D;
	*i += 2;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	len = *i - start;
	if (input[*i] == quote)
		(*i)++;
	chunk = ft_strndup(&input[start], len);
	return (chunk);
}

char	*parse_token_chunk(const char *input,
						t_parse_state *ps, t_quote *part_qt, t_mini *mini)
{
	char	*chunk;

	chunk = NULL;
	*part_qt = Q_NONE;
	if (is_dollar_quote(input, ps->i))
		chunk = handle_dollar_quote(input, &ps->i, part_qt);
	else if ((input[ps->i] == '\'' || input[ps->i] == '"') && \
							has_closing_quote(input, ps->i, input[ps->i]))
		chunk = extract_quoted(input, &ps->i, part_qt, mini);
	else if (input[ps->i] == '$')
	{
		if (!input[ps->i + 1] || !(ft_isalpha(input[ps->i + 1]) || \
							input[ps->i + 1] == '_' || input[ps->i + 1] == '?'))
		{
			*part_qt = Q_D;
			ps->i++;
			return (ft_strdup("$"));
		}
		else
			return (expand_var(input, &ps->i, mini));
	}
	else
		chunk = (extract_plain(input, &ps->i));
	return (chunk);
}

char	*build_token_arg(const char *input,
	t_parse_state *ps, t_mini *mini, t_quote *qt)
{
	char	*arg;
	char	*chunk;
	t_quote	part_qt;

	arg = ft_strdup("");
	*qt = Q_NONE;
	while (input[ps->i] && !ft_isspace(input[ps->i]) && \
								!is_meta_char(input[ps->i]))
	{
		if (arg[0] != '\0' && !ps->glued)
			break ;
		chunk = parse_token_chunk(input, ps, &part_qt, mini);
		if (!chunk)
			break ;
		if (ft_strcmp(chunk, "$") == 0)
			ps->glued = false;
		arg = ft_strjoin_ff(arg, chunk);
		if (*qt == Q_NONE)
			*qt = part_qt;
	}
	return (arg);
}

void	fill_current_token(const char *input,
	t_parse_state *ps, t_token **tokens, t_mini *mini)
{
	char	*arg;
	t_quote	qt;
	int		start_i;
	bool	glued;

	start_i = ps->i;
	arg = build_token_arg(input, ps, mini, &qt);
	glued = (start_i > 0
			&& input[start_i - 1] != ' '
			&& !is_meta_char(input[start_i - 1]));
	if (arg && (arg[0] || qt != Q_NONE))
		append_t(tokens, create_t_with_glued(arg, qt, glued));
	else
		free(arg);
}
