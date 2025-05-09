/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/09 08:54:10 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_meta_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	skip_spaces(const char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}
/* problemetic case:
bash-5.2$ echo abcd"$USER"efgh
abcdphoenixefgh

minishell> echo abcd"$USER"efgh
Token: [echo], quote_type: 0
Token: [abcd$USERefgh], quote_type: 2
abcd -> no good

add is_wrapped() to keep quote while it's in a string
minishell> echo abcd"$USER"efgh
Token: [echo], quote_type: 0
Token: [abcd"$USER"efgh], quote_type: 0
abcdphoenixefgh

*/
/* seperate in several small function */
// static char	*extract_quoted(const char *input, int *i, char *current, t_quote_type *qt)
// {
// 	char quote = input[(*i)++];
// 	int start = *i;
// 	int is_wrapped = (current[0] == '\0'); // ⬅️ 如果 quote 出現在 token 一開始
// 	while (input[*i] && input[*i] != quote)
// 		(*i)++;
// 	int len = *i - start;
// 	if (input[*i] == quote)
// 		(*i)++;
// 	char *quoted;
// 	if (is_wrapped && (input[*i] == '\0' || ft_isspace(input[*i]) || is_meta_char(input[*i])))
// 	{
// 		// quote 是獨立整段包裹 → 拿掉 quote，設定 quote_type
// 		quoted = ft_strndup(&input[start], len);
// 		if (quote == '"')
// 			*qt = QUOTE_DOUBLE;
// 		else if (quote == '\'')
// 			*qt = QUOTE_SINGLE;
// 	}
// 	else
// 	{
// 		// quote 是中間的一部分 → 保留原始 quote，整段屬於 unquoted 結構
// 		quoted = ft_strndup(&input[start - 1], len + 2);
// 		*qt = QUOTE_NONE;
// 	}
// 	return ft_strjoin_f(current, quoted);
// }

static int	get_quote_len(const char *input, int start, char quote)
{
	int	i;

	i = start;
	while (input[i] && input[i] != quote)
		i++;
	return (i - start);
}

static void	set_quote_type(char quote, t_quote_type *qt)
{
	if (quote == '"')
		*qt = QUOTE_DOUBLE;
	else if (quote == '\'')
		*qt = QUOTE_SINGLE;
}

static int	should_strip_quotes(const char *input, int i, const char *current)
{
	return (current[0] == '\0'
		&& (input[i] == '\0' || ft_isspace(input[i]) || is_meta_char(input[i])));
}

char	*extract_quoted(const char *input, int *i,
						char *current, t_quote_type *qt)
{
	char	quote;
	int		start;
	int		len;
	int		strip;
	char	*quoted;

	quote = input[(*i)++];
	start = *i;
	len = get_quote_len(input, start, quote);
	if (input[*i + len] == quote)
		*i += len + 1;
	else
		*i += len;
	strip = should_strip_quotes(input, *i, current);
	if (strip)
	{
		quoted = ft_strndup(&input[start], len);
		set_quote_type(quote, qt);
	}
	else
	{
		quoted = ft_strndup(&input[start - 1], len + 2);
		*qt = QUOTE_NONE;
	}
	return (ft_strjoin_f(current, quoted));
}
//----------------------------------
static char	*extract_plain(const char *input, int *i, char *current)
{
	int start = *i;

	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"' &&
		input[*i] != '\'' && !is_meta_char(input[*i]))
		(*i)++;
	return ft_strjoin_f(current, ft_strndup(&input[start], *i - start));
}

static int	handle_meta(const char *input, int i, t_token **tokens)
{
	int len = (input[i] == input[i + 1]) ? 2 : 1;
	append_t(tokens, create_t(ft_strndup(&input[i], len), QUOTE_NONE));
	return (i + len);
}

void	init_token_loop_vars(char **current, t_quote_type *qt)
{
	*current = ft_strdup("");
	if (!*current)
		exit_error("malloc failed in init_token_loop_vars");
	*qt = QUOTE_NONE;
}

void	fill_current_token(const char *input, int *i, char **current, t_quote_type *qt)
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

void print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: [%s], quote_type: %d\n",
			token->str[0] ? token->str : "(empty)", token->quote_type);
		token = token->next;
	}
}
