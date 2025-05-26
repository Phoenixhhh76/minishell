/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 10:43:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/26 18:43:17 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/*
** Check if current input position is a Bash-style $"..." or $'...'
** echo $"hello" -> hello
*/
bool	is_dollar_quote(const char *input, int i)
{
	return (input[i] == '$' && (input[i + 1] == '"' || input[i + 1] == '\''));
}

/*
** Handle a Bash-style $"..." or $'...' string (no variable expansion)
** Extracts the quoted string as-is and sets quote_type.
** 处理 Bash 的 $"..." 或 $'...'，不展开变量，只提取内容。
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
	*i += 2;  // Skip $ and the opening quote
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	len = *i - start;
	if (input[*i] == quote)
		(*i)++;
	chunk = ft_strndup(&input[start], len);
	//printf("[handle_dollar_quote] -> %.*s\n", len, &input[start]);
	return (chunk);
}

bool	has_closing_quote(const char *input, int i, char quote)
{
	i++;
	while (input[i])
	{
		if (input[i] == quote)
			return (true);
		i++;
	}
	return (false);
}

bool	is_token_start(const char *input, int i)
{
	return (i == 0 || ft_isspace(input[i - 1]) || is_meta_char(input[i - 1]));
}

void fill_current_token(const char *input, t_parse_state *ps, t_token **tokens, t_mini *mini)
{
	char	*arg;
	t_quote	qt;
	t_quote part_qt;
	char	*chunk;

	arg = ft_strdup("");
	qt = Q_NONE;
	while (input[ps->i] && !ft_isspace(input[ps->i]) && !is_meta_char(input[ps->i]))
	{

		if (arg[0] != '\0' && !ps->glued)//
				break; //echo "$HO"ME
		chunk = NULL;
		part_qt = Q_NONE;
		if (is_dollar_quote(input, ps->i))
		{
			chunk = handle_dollar_quote(input, &ps->i, &part_qt);
		}
		else if ((input[ps->i] == '\'' || input[ps->i] == '"') && \
							has_closing_quote(input, ps->i, input[ps->i]))
		{
			chunk = extract_quoted(input, &ps->i, &part_qt);
		}
		else if (input[ps->i] == '$')
		{
			chunk = expand_var(input, &ps->i, mini);
		}
		else
		{
			chunk = extract_plain(input, &ps->i);
		}

		if (!chunk)
			break ;
		arg = ft_strjoin_ff(arg, chunk);
		if (qt == Q_NONE)
			qt = part_qt;
	}
	if (arg[0] || qt != Q_NONE)
		append_t(tokens, create_t_with_glued(arg, qt, ps->glued));
	else
		free(arg);
	printf("[DEBUG fill] arg=[%s] qt=%d glued=%d\n", arg, qt, ps->glued);

}

/* bool is_safe_to_continue_gluing(t_quote last_qt, t_quote next_qt, bool glued)
{
	(void)next_qt;
	if (glued)
		return (true); // 如果是 glued，就继续拼接

	if (last_qt != Q_NONE)
		return (false); // 前面是引号包裹的，但现在不是 glued → 不拼

	return (false); // 默认情况下也不拼
}
*/

t_token	*create_t_with_glued(char *str, t_quote qt, bool glued)
{
	t_token	*tok;

	tok = create_t(str, qt);
	tok->glued = glued;
	return (tok);
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

void debug_tokens(t_token *tok)
{
	while (tok)
	{
		printf("Token [%s]  quote_type=%d  glued=%d\n",
			tok->str, tok->quote_type, tok->glued);
		tok = tok->next;
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
		//check_and_handle_meta(input, &ps.i, &tokens);
		if (is_meta_char(input[ps.i]))
			check_and_handle_meta(input, &ps.i, &tokens);
		if (input[ps.i] && !ft_isspace(input[ps.i]) && !is_meta_char(input[ps.i]))
			ps.glued = true;
		else
			ps.glued = false;
	}
	//debug_tokens(tokens);
	return (tokens);
}







