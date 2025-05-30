/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 19:17:17 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 11:58:09 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_export_chunk(t_token *tok, t_mini *mini)
{
	if (tok->quote_type == Q_S)
		return (ft_strdup(tok->str));
	else
		return (expand_if_needed(tok, mini));
}

char	*collect_export_chunks(t_token *tok, t_mini *mini,
								char *arg, t_token **cur_tok_ptr)
{
	char	*chunk;

	while (tok && (tok->type == CMD || tok->type == UNKNOWN))
	{
		chunk = get_export_chunk(tok, mini);
		if (!chunk)
			break ;
		if (arg[0] != '\0' && !tok->glued)
		{
			free(chunk);
			break ;
		}
		arg = ft_strjoin_ff(arg, chunk);
		if (!arg)
			break ;
		tok = tok->next;
		if (!tok || is_meta_token(tok->type))
			break ;
	}
	*cur_tok_ptr = tok;
	return (arg);
}

char	*join_tokens_for_export(t_token **cur_tok_ptr, t_mini *mini)
{
	t_token	*tok;
	char	*arg;

	tok = *cur_tok_ptr;
	arg = ft_strdup("");
	if (!arg)
		return (NULL);
	arg = collect_export_chunks(tok, mini, arg, cur_tok_ptr);
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

/*
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
} */