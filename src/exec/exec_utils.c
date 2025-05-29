/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-29 18:16:28 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-29 18:16:28 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_there_pipe(t_mini *mini)
{
	t_token	tmp;

	tmp = mini->ast->ast_tok;
	if (tmp.type == PIPE)
		return (true);
	return (false);
}

bool	has_redirection(t_cmd *cmd)
{
	if (cmd->infile || cmd->outfile || cmd->heredoc_nb > 0)
		return (true);
	return (false);
}

void	handle_exit_status(t_mini *mini, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			mini->last_exit = 130;
		else if (WTERMSIG(status) == SIGQUIT)
			mini->last_exit = 131;
	}
	else if (WIFEXITED(status))
		mini->last_exit = WEXITSTATUS(status);
}
