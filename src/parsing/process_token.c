/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:40:11 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/30 12:04:19 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_token(char **args, int i, t_token *tok, t_mini *mini)
{
	if (tok->quote_type == Q_S)
		return (handle_single(args, i, tok));
	return (handle_expanded(args, i, tok, mini));
}

int	handle_single(char **args, int i, t_token *tok)
{
	args[i++] = ft_strdup(tok->str);
	return (i);
}

int	add_split(char **args, int i, char *expanded)
{
	char	**split;
	int		j;

	split = ft_split(expanded, ' ');
	j = 0;
	while (split && split[j])
		args[i++] = ft_strdup(split[j++]);
	free(expanded);
	free_double_tab(split);
	return (i);
}

/*
the condition strchr ", make sure if we have a string like
$HOLA="  bonjour he ",have space at the beginning and end
Token [wesh"$HOLA"]  quote_type 0, we won't split it, we keep "  bonjour he "
 */

int	handle_expanded(char **args, int i, t_token *tok, t_mini *mini)
{
	char	*expanded;

	expanded = expand_if_needed(tok, mini);
	if (!expanded)
		return (i);
	if (tok->quote_type == Q_D || ft_strchr(tok->str, '"') || \
		tok->quote_type == Q_S || ft_strchr(tok->str, '\''))
		args[i++] = expanded;
	else
		i = add_split(args, i, expanded);
	return (i);
}
