/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_57.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:33:52 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:17:13 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/* V0507, problem whil "" at begining of cmd line */
static int	process_redirection(const char *input, int i, t_token **tokens)
{
	int	len;

	len = 1;
	if (input[i] == input[i + 1])
		len = 2;
	append_t(tokens, create_t(ft_strndup(&input[i], len), Q_NONE));
	return (i + len);
}

static int	process_token(const char *input, int i, t_token **tokens)
{
	int		start;
	char	quote;
	char	*content;

	if (input[i] == '>' || input[i] == '<')
		return (process_redirection(input, i, tokens));
	else if (input[i] == '|')
	{
		append_t(tokens, create_t(ft_strndup(&input[i], 1), Q_NONE));
		return (i + 1);
	}
	else if (input[i] == '"' || input[i] == '\'')
	{
		quote = input[i++];
		start = i;
		while (input[i] && input[i] != quote)
			i++;
		content = ft_strndup(input + start, i - start);
		if (input[i] == quote)
			i++;
		append_t(tokens, create_t(content, quote == '"' ? Q_D : Q_S));
		return (i);
	}
	else
	{
		start = i;
		while (input[i] && !ft_isspace(input[i]) && input[i] != '|' && input[i] != '<' && input[i] != '>')
			i++;
		append_t(tokens, create_t(ft_strndup(&input[start], i - start), Q_NONE));
		return (i);
	}
}


//for ""empty, but not work for """"echo hola""", permission denied
t_token	*tokenize_input(const char *input)
{
	t_quote qt;
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;

		if (!input[i])
			break;

		// Special case: "" or '' (empty string token)
		if ((input[i] == '"' && input[i + 1] == '"' && input[i + 2] == ' ') ||
			(input[i] == '\'' && input[i + 1] == '\'' && input[i + 2] == ' '))
		{
			if (input[i] == '"')
				qt = Q_D;
			else
				qt = Q_S;
			i += 2;
			append_t(&tokens, create_t(ft_strdup(""), qt));
			continue;
		}
		i = process_token(input, i, &tokens);
	}

	return tokens;
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
