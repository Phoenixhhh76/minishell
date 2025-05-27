/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:05:22 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/27 18:10:55 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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


// char	**collect_args(t_token *start, t_token *end, t_mini *mini)
// {
// 	int		size;
// 	char	**args;
// 	int		i;
// 	t_token	*prev;
// 	t_token	*scan;

// 	i = 0;
// 	size = count_args_advanced(start, end, mini);
// 	args = ft_calloc(size + 1, sizeof(char *));
// 	if (!args)
// 		return NULL;
// 	while (start && start != end)
// 	{
// 		if (start->quote_type != Q_NONE && (!start->str || start->str[0] == '\0'))
// 		{
// 			args[i++] = ft_strdup("");
// 			start = start->next;
// 			continue ;
// 		}
// 		if (start->type == CMD || start->type == UNKNOWN)
// 		{
// 			prev = start;
// 			scan = start;
// 			char *arg = join_tokens_for_arg(&start, mini, false);
// 			// fallback: 如果 join 没前进，就手动推进
// 			if (start == prev)
// 				start = start->next;
// 			// 判断拼接内容中是否包含非空的 quoted token
// 			bool quoted = false;
// 			while (scan && scan != start)
// 			{
// 				if (scan->quote_type != Q_NONE && scan->str && scan->str[0] != '\0')
// 				{
// 					quoted = true;
// 					break;
// 				}
// 				scan = scan->next;
// 			}
// 			if (quoted)
// 			{
// 				// 即使是空，也要保留
// 				args[i++] = (arg && arg[0]) ? arg : ft_strdup("");
// 			}
// 			else
// 			{
// 				// 拆分逻辑
// 				char **split = ft_split(arg, ' ');
// 				free(arg);
// 				if (!split || !split[0])
// 				{
// 					args[i++] = ft_strdup("");  // 保留空字符串参数
// 					free_double_tab(split);
// 				}
// 				else
// 				{
// 					for (int j = 0; split[j]; j++)
// 						args[i++] = ft_strdup(split[j]);
// 					free_double_tab(split);
// 				}
// 			}
// 		}
// 		else if (is_redirection(start->type) && start->next)
// 		{
// 			start = start->next->next;
// 		}
// 		else
// 		{
// 			start = start->next;
// 		}
// 	}
// 	args[i] = NULL;
// 	return args;
// }
static void	process_split_argument(char ***args, int *i, char *arg)
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

