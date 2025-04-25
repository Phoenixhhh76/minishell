/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:33:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/25 17:59:01 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_quoted_token(const char *input, int i, t_token **tokens)
{
	char	quote;
	int		start;

	quote = input[i];
	start = ++i;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
	{
		if (quote == '\'')
			append_t(tokens, \
				create_t(ft_strndup(&input[start], i - start), QUOTE_SINGLE));
		else
			append_t(tokens, \
				create_t(ft_strndup(&input[start], i - start), QUOTE_DOUBLE));
		return (i + 1);
	}
	/* if quote is not in pair, we put QUOTE_NONE in the moment */
	append_t(tokens, \
		create_t(ft_strndup(&input[start - 1], i - start + 1), QUOTE_NONE));
	return (i);
}

static int	process_redirection(const char *input, int i, t_token **tokens)
{
	int	len;

	len = 1;
	if (input[i] == input[i + 1])
		len = 2;
	append_t(tokens, create_t(ft_strndup(&input[i], len), QUOTE_NONE));
	return (i + len);
}

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
}


/* static int	process_token(const char *input, int i, t_token **tokens)
{
	int	start;

	if ((input[i] == '"' || input[i] == '\'') && find_matching_quote(&input[i]))
		return (process_quoted_token(input, i, tokens));
	else if (input[i] == '>' || input[i] == '<')
		return (process_redirection(input, i, tokens));
	else if (input[i] == '|')
	{
		append_t(tokens, create_t(ft_strndup(&input[i], 1), QUOTE_NONE));
		return (i + 1);
	}
	else
	{
		start = i;
		while (input[i] && !ft_isspace(input[i])
			&& input[i] != '|' && input[i] != '<' && input[i] != '>'
			&& input[i] != '"' && input[i] != '\'')//remove '\'' here
			i++;
		append_t(tokens, \
			create_t(ft_strndup(&input[start], i - start), QUOTE_NONE));
		return (i);
	}
} */

static int	process_token(const char *input, int i, t_token **tokens)
{
	int	start;

	if ((input[i] == '"' || input[i] == '\'') && find_matching_quote(&input[i]))
	//if ((input[i] == '"') && find_matching_quote(&input[i]))
		return (process_quoted_token(input, i, tokens));
	else if (input[i] == '>' || input[i] == '<')
		return (process_redirection(input, i, tokens));
	else if (input[i] == '|')
	{
		append_t(tokens, create_t(ft_strndup(&input[i], 1), QUOTE_NONE));
		return (i + 1);
	}
	else
	{
		start = i;
		// while (input[i] && !ft_isspace(input[i])
		// 	&& input[i] != '|' && input[i] != '<' && input[i] != '>')
		while (input[i] && !ft_isspace(input[i])
			&& input[i] != '|' && input[i] != '<' && input[i] != '>'
			&& input[i] != '"' && input[i] != '\'')
			i++;
		append_t(tokens, \
			create_t(ft_strndup(&input[start], i - start), QUOTE_NONE));
		return (i);
	}
}

t_token	*tokenize_input(const char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		i = process_token(input, i, &tokens);
	}
	return (tokens);
}
void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: [%s], quote_type: %d\n",
			token->str[0] ? token->str : "(empty)", token->quote_type);
		token = token->next;
	}
}
