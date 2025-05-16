/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:37:11 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 11:13:40 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_node	get_token_type(const char *str)
{
	if (!strcmp(str, "|"))
		return (PIPE);
	if (!strcmp(str, ">"))
		return (R_OUT);
	if (!strcmp(str, "<"))
		return (R_IN);
	if (!strcmp(str, ">>"))
		return (R_A);
	if (!strcmp(str, "<<"))
		return (HD);
	return (CMD);
}

t_token	*create_t(char *str, t_quote quote_type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str);
	new->quote_type = quote_type;
	new->next = NULL;
	return (new);
}

void	append_t(t_token **head, t_token *new)
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

/*
void	print_token_list(t_token *token)
{
	while (token)
	{
		printf("Token: %-10s Type: %d\n", token->str, token->type);
		token = token->next;
	}
} */
