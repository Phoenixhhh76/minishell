/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/31 13:00:08 by hho-troc         ###   ########.fr       */
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

// void	fill_current_token(const char *input,
// 						t_parse_state *ps, t_token **tokens, t_mini *mini)
// {
// 	char	*arg;
// 	t_quote	qt;
// 	t_quote	part_qt;
// 	char	*chunk;

// 	arg = ft_strdup("");
// 	qt = Q_NONE;
// 	while (input[ps->i] && !ft_isspace(input[ps->i]) &&
// 									!is_meta_char(input[ps->i]))
// 	{
// 		if (arg[0] != '\0' && !ps->glued)
// 			break ;
// 		chunk = parse_token_chunk(input, ps, &part_qt, mini);
// 		if (!chunk)
// 			break ;
// 		if (ft_strcmp(chunk, "$") == 0)
// 			ps->glued = false;
// 		arg = ft_strjoin_ff(arg, chunk);
// 		if (qt == Q_NONE)
// 			qt = part_qt;
// 	}
// 	printf("[fill_token] FINAL TOKEN='%s'  qt=%d  glued=%d\n", arg, qt, ps->glued);
// 	if (arg && (arg[0] || qt != Q_NONE))
// 		append_t(tokens, create_t_with_glued(arg, qt, ps->glued));
// 	else
// 		free(arg);
// }

void	fill_current_token(const char *input,
	t_parse_state *ps, t_token **tokens, t_mini *mini)
{
	char	*arg;
	t_quote	qt;
	t_quote	part_qt;
	char	*chunk;

	int		start_i = ps->i;    // ✅ 初始化
	bool	glued;               // ✅ 宣告

	arg = ft_strdup("");
	qt = Q_NONE;

	while (input[ps->i] && !ft_isspace(input[ps->i]) && !is_meta_char(input[ps->i]))
	{
	if (arg[0] != '\0' && !ps->glued)
	break ;
	chunk = parse_token_chunk(input, ps, &part_qt, mini);
	if (!chunk)
	break ;
	if (ft_strcmp(chunk, "$") == 0)
	ps->glued = false;
	arg = ft_strjoin_ff(arg, chunk);
	if (qt == Q_NONE)
	qt = part_qt;
	}

	glued = (start_i > 0 && input[start_i - 1] != ' ' && !is_meta_char(input[start_i - 1]));

	printf("[fill_token] FINAL TOKEN='%s'  qt=%d  glued=%d\n", arg, qt, glued);

	if (arg && (arg[0] || qt != Q_NONE))
	append_t(tokens, create_t_with_glued(arg, qt, glued));  // ✅ 使用 glued
	else
	free(arg);
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
void	print_token_list(t_token *tokens)
{
	while (tokens)
	{
		printf("Token [%s] type=%d quote=%d glued=%d\n",
			tokens->str, tokens->type, tokens->quote_type, tokens->glued);
		tokens = tokens->next;
	}
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
		// if (input[ps.i] && !ft_isspace(input[ps.i]) &&
		// 							!is_meta_char(input[ps.i]))
		// 	ps.glued = true;
		// else
		// 	ps.glued = false;
	}
	print_token_list(tokens);
	return (tokens);
}
