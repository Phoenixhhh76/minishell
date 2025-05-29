int	count_args_advanced(t_token *start, t_token *end, t_mini *mini)
{
	int	count;

	count = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			count += count_token_args(start, mini);
		else if ((start->type == R_IN || start->type == R_OUT || \
				start->type == R_A || start->type == HD) && start->next)
			start = start->next;
		start = start->next;
	}
	return (count);
}

char	*join_tokens_for_arg(t_token **cur_tok_ptr, t_mini *mini, bool allow_split)
{
	t_token	*tok;
	char	*arg;
	t_quote	prev_quote;
	char	*expanded;
	char	*tmp;
	(void)	allow_split;

	tok = *cur_tok_ptr;
	arg = ft_strdup("");
	prev_quote = Q_NONE;
	if (!arg)
		return (NULL);
	while (tok && (tok->type == CMD || tok->type == UNKNOWN))
	{
		expanded = expand_if_needed(tok, mini);
		if (!expanded)
			break ;
		if (arg[0] != '\0' && !tok->glued && prev_quote != Q_NONE)
		{
			free(expanded);
			break ;
		}
		if (arg[0] != '\0' && !tok->glued)
		{
			free(expanded);
			break ;
		}
		tmp = ft_strjoin(arg, expanded);
		free(arg);
		free(expanded);
		arg = tmp;
		prev_quote = tok->quote_type;
		tok = tok->next;

		if (!tok || is_meta_token(tok->type))
			break ;
	}
	*cur_tok_ptr = tok;
	return (arg);
}
tatic void	process_split_argument(char ***args, int *i, char *arg)
{
	char	**split;
	int		j;

	split = ft_split(arg, ' ');
	free(arg);
	if (!split || !split[0])
	{
		(*args)[(*i)++] = ft_strdup("");
		free_double_tab(split);
		return ;
	}
	j = 0;
	while (split[j])
	{
		(*args)[(*i)++] = ft_strdup(split[j]);
		j++;
	}
	free_double_tab(split);
}

static bool	should_preserve_empty_arg(t_token *from, t_token *to)
{
	while (from && from != to)
	{
		if (from->quote_type != Q_NONE && from->str && from->str[0] != '\0')
			return (true);
		from = from->next;
	}
	return (false);
}

static void	process_argument_token(char ***args, int *i, t_token **start_ptr, t_mini *mini)
{
	t_token	*prev;
	t_token	*scan;
	bool	quoted;
	char	*arg;

	prev = *start_ptr;
	scan = *start_ptr;
	arg = join_tokens_for_arg(start_ptr, mini, false);
	if (*start_ptr == prev)
		*start_ptr = prev->next;
	quoted = should_preserve_empty_arg(scan, *start_ptr);
	if (quoted)
	{
		if (arg && arg[0])
			(*args)[(*i)++] = arg;
		else
		{
			(*args)[(*i)++] = ft_strdup("");
			free(arg);
		}
	}
	else
		process_split_argument(args, i, arg);
}

static bool	handle_quoted_empty_token(char **arg_slot, t_token **start_ptr)
{
	t_token	*start;

	start = *start_ptr;
	if (start->quote_type != Q_NONE && (!start->str || start->str[0] == '\0'))
	{
		*arg_slot = ft_strdup("");
		*start_ptr = start->next;
		return (true);
	}
	return (false);
}

char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		size;
	char	**args;
	int		i;

	i = 0;
	size = count_args_advanced(start, end, mini);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (NULL);
	while (start && start != end)
	{
		if (handle_quoted_empty_token(&args[i], &start))
		{
			i++;
			continue ;
		}
		if (start->type == CMD || start->type == UNKNOWN)
			process_argument_token(&args, &i, &start, mini);
		else if (is_redirection(start->type) && start->next)
			start = start->next->next;
		else
			start = start->next;
	}
	args[i] = NULL;
	return (args);
}


char	**collect_args_for_export(t_token *start, t_token *end, t_mini *mini)
{
	int		count;
	char	**args;

	count = count_export_args(start, end, mini);
	args = (char **)ft_calloc(count + 1, sizeof(char *));
	if (!args)
		return (NULL);
	fill_export_args(args, start, end, mini);
	return (args);
}
int	count_token_args(t_token *tok, t_mini *mini)
{
	char	*expanded;
	int		count;

	count = 0;
	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (0);
	if (tok->quote_type == Q_S || tok->quote_type == Q_D)
		count = 1;
	else if (expanded[0] != '\0')
		count = count_expanded_split(expanded);
	else
		count = 0;
	free(expanded);
	return (count);
}

int	count_expanded_split(char *expanded)
{
	char	**split;
	int		count;
	int		i;

	count = 0;
	i = 0;
	split = ft_split(expanded, ' ');
	while (split && split[i])
	{
		count++;
		i++;
	}
	free_double_tab(split);
	return (count);
}

int	count_export_args(t_token *start, t_token *end, t_mini *mini)
{
	int			count;
	char		*expanded;
	t_token		*tmp;

	count = 0;
	tmp = start;
	while (tmp && tmp != end)
	{
		if (tmp->type == CMD || tmp->type == UNKNOWN)
		{
			expanded = expand_if_needed(tmp, mini);
			if (expanded && (expanded[0] != '\0' || tmp->quote_type != Q_NONE))
				count++;
			free(expanded);
		}
		tmp = tmp->next;
	}
	return (count);
}

char	*join_tokens_for_export(t_token **cur_tok_ptr, t_mini *mini)
{
	t_token	*tok;
	char	*arg;
	char	*tmp;
	char	*chunk;

	tok = *cur_tok_ptr;
	arg = ft_strdup("");
	if (!arg)
		return (NULL);
	while (tok && (tok->type == CMD || tok->type == UNKNOWN))
	{
		if (tok->quote_type == Q_S)
			chunk = ft_strdup(tok->str);
		else
			chunk = expand_if_needed(tok, mini);
		if (!chunk)
			break ;
		if (arg[0] != '\0' && !tok->glued)
		{
			free(chunk);
			break ;
		}
		tmp = ft_strjoin(arg, chunk);
		free(arg);
		free(chunk);
		arg = tmp;
		tok = tok->next;
		if (!tok || is_meta_token(tok->type))
			break ;
	}
	*cur_tok_ptr = tok;
	return (arg);
}



void	fill_export_args(char **args,
			t_token *start, t_token *end, t_mini *mini)
{
	int	i;

	i = 0;
	while (start && start != end)
	{
		if (start->type == CMD || start->type == UNKNOWN)
			args[i++] = join_tokens_for_export(&start, mini);
		else
			start = start->next;
	}
	args[i] = NULL;
}
int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	//printf("[DEBUG] write args[%d] = %s\n", i, args[i]);

	if (tok->quote_type == Q_S)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}

int	handle_single(char **args, int i, t_token *tok)
{
	args[i++] = ft_strdup(tok->str);
	return (i);
}

int	add_split(char **args, int i, char *expanded)
{
	char	**split;
	int		j;

	split = ft_split(expanded, ' ');
	j = 0;
	while (split && split[j])
		args[i++] = ft_strdup(split[j++]);
	free(expanded);
	free_double_tab(split);
	return (i);
}

/*
the condition strchr ", make sure if we have a string like
$HOLA="  bonjour he ",have space at the beginning and end
Token [wesh"$HOLA"]  quote_type 0, we won't split it, we keep "  bonjour he "
 */

int	handle_expanded(char **args, int i, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (i);
	if (tok->quote_type == Q_D || ft_strchr(tok->str, '"') ||
		tok->quote_type == Q_S || ft_strchr(tok->str, '\''))
		args[i++] = expanded;  // even if "", still added
	else
		i = add_split(args, i, expanded);
	return (i);
}




