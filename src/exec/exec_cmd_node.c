/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-29 18:40:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-29 18:40:52 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_cmd_errors(t_mini *mini, t_cmd *cmd)
{
	if (!cmd->cmd_args || !cmd->cmd_args[0] || cmd->cmd_args[0][0] == '\0')
	{
		mini->last_exit = err_msg("", ":", " command not found", 127);
		cmd->path_error = 1;
	}
}

static void	check_cmd_path_access(t_mini *mini, t_cmd *cmd)
{
	if (access(cmd->cmd_path, F_OK) != 0)
	{
		err_msg(cmd->cmd_path, ":", "  command not found", 127);
		safe_exit(mini, 127);
	}
	if (access(cmd->cmd_path, X_OK) != 0)
	{
		err_msg(cmd->cmd_path, ":", " Permission denied", 126);
		safe_exit(mini, 126);
	}
}

static void	try_execve(t_mini *mini, t_cmd *cmd, char **envp)
{
	execve(cmd->cmd_path, cmd->cmd_args, envp);
	if (access(cmd->cmd_path, X_OK) == 0)
	{
		perror(cmd->cmd_args[0]);
		safe_exit(mini, 126);
	}
	mini->last_exit = err_msg(cmd->cmd_args[0], ":", " command not found", 127);
}

void	exec_cmd_node(t_mini *mini, t_ast *node, char **envp)
{
	t_cmd	*cmd;

	cmd = node->cmd;
	if (!cmd)
		safe_exit(mini, mini->last_exit);
	if (cmd->in_error == 1 || cmd->path_error == 1)
		safe_exit(mini, 1);
	handle_redirects(cmd);
	if ((!cmd->append && !cmd->heredoc_nb && !cmd->infile
			&& !cmd->outfile) && (!cmd->cmd_args || !cmd->cmd_args[0]
			|| cmd->cmd_args[0][0] == '\0'))
	{
		mini->last_exit = err_msg("", ":", " command not found", 127);
		cmd->path_error = 1;
	}
	if (cmd->cmd_args && cmd->cmd_path)
	{
		handle_cmd_errors(mini, cmd);
		check_cmd_path_access(mini, cmd);
		try_execve(mini, cmd, envp);
	}
	safe_exit(mini, mini->last_exit);
}
