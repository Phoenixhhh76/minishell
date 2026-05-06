/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_tokens_arg.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 19:08:42 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/01 11:36:37 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_break(char *arg, t_token *tok, t_quote prev_quote)
{
	(void)prev_quote;
	if (arg[0] == '\0')
		return (false);
	if (tok->glued)
		return (false);
	if (ft_strcmp(arg, "$") == 0 && tok->quote_type == Q_NONE && \
												ft_isalpha(tok->str[0]))
		return (true);
	return (true);
}

char	*collect_tokens(char *arg, t_token **tok, t_mini *mini)
{
	t_quote	prev_quote;
	char	*expanded;

	prev_quote = Q_NONE;
	while (*tok && ((*tok)->type == CMD || (*tok)->type == UNKNOWN))
	{
		expanded = expand_if_needed(*tok, mini);
		if (!expanded)
			break ;
		if (should_break(arg, *tok, prev_quote))
		{
			free(expanded);
			break ;
		}
		arg = ft_strjoin_ff(arg, expanded);
		if (!arg)
			break ;
		prev_quote = (*tok)->quote_type;
		*tok = (*tok)->next;
		if (!(*tok) || is_meta_token((*tok)->type))
			break ;
	}
	return (arg);
}

char	*join_tokens_for_arg(t_token **cur, t_mini *mini, bool allow_split)
{
	char	*arg;

	(void)allow_split;
	arg = ft_strdup("");
	if (!arg)
		return (NULL);
	arg = collect_tokens(arg, cur, mini);
	return (arg);
}

/*
char	*join_tokens_for_arg(t_token **cur_tok_ptr,
									t_mini *mini, bool allow_split)
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
} */
