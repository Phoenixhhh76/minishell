/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:37:11 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/31 12:02:13 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// t_node	get_token_type(const char *str)
// {
// 	if (!ft_strcmp(str, "|"))
// 		return (PIPE);
// 	if (!ft_strcmp(str, ">"))
// 		return (R_OUT);
// 	if (!ft_strcmp(str, "<"))
// 		return (R_IN);
// 	if (!ft_strcmp(str, ">>"))
// 		return (R_A);
// 	if (!ft_strcmp(str, "<<"))
// 		return (HD);
// 	return (CMD);
// }

t_node	get_token_type(const char *str, t_quote qt)
{
	if (qt == Q_NONE)
	{
		if (!ft_strcmp(str, ">"))
			return (R_OUT);
		if (!ft_strcmp(str, "<"))
			return (R_IN);
		if (!ft_strcmp(str, ">>"))
			return (R_A);
		if (!ft_strcmp(str, "<<"))
			return (HD);
		if (!ft_strcmp(str, "|"))
			return (PIPE);
	}
	return (CMD);
}


// t_token	*create_t(char *str, t_quote quote_type)
// {
// 	t_token	*new;

// 	new = malloc(sizeof(t_token));
// 	if (!new)
// 		return (NULL);
// 	new->str = str;
// 	new->type = get_token_type(str);
// 	new->quote_type = quote_type;
// 	new->is_dollar_quote = false;
// 	new->next = NULL;
// 	return (new);
// }

t_token	*create_t(char *str, t_quote quote_type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str, quote_type); // 這裡有差
	new->quote_type = quote_type;
	new->is_dollar_quote = false;
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

t_token	*create_t_with_glued(char *str, t_quote qt, bool glued)
{
	t_token	*tok;

	tok = create_t(str, qt);
	tok->glued = glued;
	return (tok);
}
