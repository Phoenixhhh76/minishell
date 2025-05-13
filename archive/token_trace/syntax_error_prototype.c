/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:06:56 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/30 15:56:43 by hho-troc         ###   ########.fr       */
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
/*
bool	check_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!cur)
		return (true);// no arg consider as valide in bash
	if (cur->type == PIPE) //| can't be the first token
		return (err_msg("syntax error", "unexpected token", "pipe", 2), false);
	while (cur)
	{
		if (cur->type == PIPE)
		{
			if (!cur->next || cur->next->type == PIPE)
				return (err_msg("syntax error", "unexpected token", "pipe", 2), false);
			//if (!cur->next)
			//	return (err_msg("syntax error", "unexpected EOF", "", 2), false);
		}
		if (is_redirection(cur->type))
		{
			if (!cur->next || cur->next->type != CMD)
				return (err_msg("syntax error", "unexpected token", cur->str, 2), false);
		}
		cur = cur->next;
	}
	return (true);
} */

bool	check_syntax(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (!cur)
		return (true);// empty line is valide
	if (cur->type == PIPE)
		return (syntax_err_msg("unexpected token ", "pipe", 2), false);
	if (is_redirection(cur->type) && (!cur->next || is_meta_token(cur->next->type)))
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
/* #include "../includes/minishell.h"

int	check_invalid_operators(const char *line)
{
	int	i = 0;

	while (line[i])
	{
		if ((line[i] == '>' && line[i + 1] == '>' && line[i + 2] == '>') ||
			(line[i] == '<' && line[i + 1] == '<' && line[i + 2] == '<'))
		{
			printf("minishell: syntax error near unexpected token `");
			write(1, &line[i], 3);
			printf("'\n");
			return (1);
		}
		if (line[i] == '|' && line[i + 1] == '|' && line[i + 2] == '|')
		{
			printf("minishell: syntax error near unexpected token `||'\n");
			return (1);
		}
		i++;
	}
	return (0);
} */
/*
bool	find_matching_quote(const char *str)
{
	char	quote;
	int		i;

	if (!str || (*str != '\'' && *str != '"'))
		return (false);

	quote = *str;
	i = 1;
	while (str[i])
	{
		if (str[i] == quote)
			return (true);
		i++;
	}
	return (false);
} */


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


