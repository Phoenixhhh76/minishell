/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:09:13 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/01 11:39:56 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*collect_cmd(t_token **start, t_mini *mini)
{
	char	*cmd;

	cmd = ft_strdup("");
	if (!cmd)
		return (NULL);
	cmd = collect_tokens(cmd, start, mini);
	return (cmd);
}

static int	collect_args_only(char **args,
							t_token *start, t_token *end, t_mini *mini)
{
	int	i;

	i = 0;
	while (start && start != end)
	{
		if (handle_quoted_empty_token(&args[i], &start))
		{
			i++;
			continue ;
		}
		if (start->type == CMD || start->type == UNKNOWN)
			process_argument_token(&args, &i, &start, mini);
		else if (is_redirection(start->type) && start->next)
			start = start->next->next;
		else
			start = start->next;
	}
	return (i);
}

char	**collect_args(t_token *start, t_token *end, t_mini *mini)
{
	int		size;
	char	**args;
	int		i;
	char	*cmd;

	size = count_args_advanced(start->next, end, mini);
	args = ft_calloc(size + 2, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	cmd = collect_cmd(&start, mini);
	if (cmd)
		args[i++] = cmd;
	i += collect_args_only(&args[i], start, end, mini);
	args[i] = NULL;
	return (args);
}
