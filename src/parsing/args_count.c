/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:38:14 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/27 16:45:15 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
