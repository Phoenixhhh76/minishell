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


void fill_current_token(const char *input, t_parse_state *ps, t_token **tokens, t_mini *mini)
{
	char	*arg;
	t_quote	qt;
	t_quote	part_qt;
	char	*chunk;

	arg = ft_strdup("");
	qt = Q_NONE;
	while (input[ps->i] && !ft_isspace(input[ps->i]) && !is_meta_char(input[ps->i]))
	{
		if (arg[0] != '\0' && !ps->glued)
				break ; //echo "$HO"ME
		chunk = NULL;
		part_qt = Q_NONE;
		if (is_dollar_quote(input, ps->i))
		{
			chunk = handle_dollar_quote(input, &ps->i, &part_qt);
		}
		else if ((input[ps->i] == '\'' || input[ps->i] == '"') && \
							has_closing_quote(input, ps->i, input[ps->i]))
		{
			chunk = extract_quoted(input, &ps->i, &part_qt, mini);
		}
		else if (input[ps->i] == '$')
		{
			chunk = expand_var(input, &ps->i, mini);
		}
		else
			chunk = extract_plain(input, &ps->i);
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
	//printf("[DEBUG fill] arg=[%s] qt=%d glued=%d\n", arg, qt, ps->glued);
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
t_node	get_token_type(const char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, ">"))
		return (R_OUT);
	if (!strcmp(str, "<"))
		return (R_IN);
	if (!strcmp(str, ">>"))
		return (R_A);
	if (!strcmp(str, "<<"))
		return (HD);
	return (CMD);
}

t_token	*create_t(char *str, t_quote quote_type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str);
	new->quote_type = quote_type;
	new->is_dollar_quote = false;
	new->next = NULL;
	return (new);
}

void	append_t(t_token **head, t_token *new)
{
	t_token	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

void	free_token_list(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		free(tmp->str);
		free(tmp);
	}
}
bool	is_redirection(t_node type)
{
	return (type == R_IN || type == R_OUT
		|| type == R_A || type == HD);
}

bool	is_meta_token(t_node type)
{
	return (type == PIPE || is_redirection(type));
}

bool	check_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!cur)
		return (true);
	if (cur->type == PIPE)
		return (syntax_err_msg("unexpected token ", "pipe", 2), false);
	if (is_redirection(cur->type) && \
				(!cur->next || is_meta_token(cur->next->type)))
		return (syntax_err_msg("unexpected redirection ", "", 2), false);
	while (cur)
	{
		if (cur->type == PIPE)
		{
			if (!cur->next || cur->next->type == PIPE)
				return (syntax_err_msg("unexpected token ", "pipe", 2), false);
		}
		if (is_redirection(cur->type))
		{
			if (!cur->next || is_meta_token(cur->next->type))
				return (syntax_err_msg("unexpected redirection", "", 2), false);
		}
		cur = cur->next;
	}
	return (true);
}

int	check_unclosed_quotes(const char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == quote)
				i++;
			else
			{
				printf("minishell: syntax error unclosed quote `%c`\n", quote);
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

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
char	*extract_quoted(const char *input, int *i, t_quote *qt)
{
	char	quote ;
	int		start;
	int		len;
	char	*chunk;

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