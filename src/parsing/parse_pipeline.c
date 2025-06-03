/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:14:49 by hho-troc          #+#    #+#             */
/*   Updated: 2025/06/01 11:37:23 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_next_pipe(t_token *start, t_token *end)
{
	while (start && start != end)
	{
		if (start->type == PIPE)
			return (start);
		start = start->next;
	}
	return (NULL);
}

int	parse_pipeline(t_token *start, t_token *end, t_mini *mini, t_ast *node)
{
	t_token	*pipe_pos;

	if (!start || start == end || !node)
		return (0);
	pipe_pos = find_next_pipe(start, end);
	if (pipe_pos)
	{
		if (!start || start == pipe_pos || !pipe_pos->next \
			|| pipe_pos->next == end)
			return (err_msg("syntax error ", "near unexpected token ", \
				"pipe", 2), 2);
		if (init_pipe_node(node) == -2)
			return (-2);
		if (handle_left_branch(start, pipe_pos, mini, node) == -2)
			return (-2);
		if (handle_right_branch(pipe_pos, end, mini, node) == -2)
			return (-2);
	}
	else
		return (handle_cmd_node(start, end, mini, node));
	return (0);
}
