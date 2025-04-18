/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:17:11 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/18 20:54:11 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_token(const char *input, int i, t_token **tokens);
static int	process_quoted_token(const char *input, int i, t_token **tokens);
static int	process_redirection(const char *input, int i, t_token **tokens);


static t_node_type	get_token_type(const char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, ">"))
		return (REDIR_OUT);
	if (!strcmp(str, "<"))
		return (REDIR_IN);
	if (!strcmp(str, ">>"))
		return (REDIR_APPEND);
	if (!strcmp(str, "<<"))
		return (HEREDOC);
	return (CMD);
}

static t_token	*create_t(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str);
	new->next = NULL;
	return (new);
}

static void	append_t(t_token **head, t_token *new)
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

#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	return (0);
}

char	*ft_strdup(const char *s1)
{
	char	*new;
	size_t	i;

	new = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
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

static int	process_quoted_token(const char *input, int i, t_token **tokens)
{
	char	quote; //we need to take care about "" '' later
	int		start;

	quote = input[i];
	start = ++i;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
	{
		append_t(tokens, create_t(ft_strndup(&input[start], i - start)));
		return (i + 1); // 跳過右引號
	}
	// if we cant fint the seconde quote, we make token as well
	append_t(tokens, create_t(ft_strndup(&input[start - 1], i - start + 1)));
	return (i);
}

static int	process_redirection(const char *input, int i, t_token **tokens)
{
	int	len;

	len = 1;
	if (input[i] == input[i + 1])
		len = 2;
	append_t(tokens, create_t(ft_strndup(&input[i], len)));
	return (i + len);
}

static int	process_token(const char *input, int i, t_token **tokens)
{
	int	start;

	if (input[i] == '"' || input[i] == '\'')
		return (process_quoted_token(input, i, tokens));
	else if (input[i] == '>' || input[i] == '<')
		return (process_redirection(input, i, tokens));
	else if (input[i] == '|')
	{
		append_t(tokens, create_t(ft_strndup(&input[i], 1)));
		return (i + 1);
	}
	else
	{
		start = i;
		while (input[i] && !ft_isspace(input[i])
			&& input[i] != '|' && input[i] != '<' && input[i] != '>'
			&& input[i] != '"' && input[i] != '\'')
			i++;
		append_t(tokens, create_t(ft_strndup(&input[start], i - start)));
		return (i);
	}
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

void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s Type: %d\n", token->str, token->type);
		token = token->next;
	}
}
/* int	main(void)
{
	//t_token *tokens = tokenize_input("echo \"hello | world\" > output.txt | cat -e");
	//t_token *tokens = tokenize_input("echo hello | world >>>> output.txt | cat -e");
	t_token *tokens = tokenize_input("echo hello | world > > > > output.txt | cat -e");
	print_token_list(tokens);
	free_token_list(tokens);
	return(0);
}

 */