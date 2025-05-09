/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:06:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/09 09:20:51 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_redirection(t_node_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == HEREDOC);
}

bool	is_meta_token(t_node_type type)
{
	return (type == PIPE || is_redirection(type));
}

bool	check_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!cur)
		return (true);// empty line is valide
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

