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

#include "minishell.h"

/* Checks if inexistant relative or abs path + cmd without correct path*/

static void	handle_cmd_errors(t_mini *mini, t_cmd *cmd)
{
	if (!cmd->cmd_args || !cmd->cmd_args[0] || cmd->cmd_args[0][0] == '\0')
	{
		mini->last_exit = err_msg("", ":", " command not found", 127);
		cmd->path_error = 1;
	}
	if (ft_strchr(cmd->cmd_args[0], '/'))
	{
		if (access(cmd->cmd_args[0], F_OK) != 0)
		{
			mini->last_exit = err_msg(cmd->cmd_args[0], ":", \
				" No such file or directory", 127);
			safe_exit(mini, mini->last_exit);
		}
	}
	else if (!cmd->cmd_path || access(cmd->cmd_path, F_OK) != 0)
	{
		mini->last_exit = err_msg(cmd->cmd_args[0], ":", \
			" command not found", 127);
		safe_exit(mini, mini->last_exit);
	}
}

static void	ft_execve(t_mini *mini, t_cmd *cmd, char **envp)
{
	struct stat	sb;
	int			err;

	execve(cmd->cmd_path, cmd->cmd_args, envp);
	err = errno;
	if (stat(cmd->cmd_path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		mini->last_exit = err_msg(cmd->cmd_path, ":", "  Is a directory", 126);
		safe_exit(mini, mini->last_exit);
	}
	else if (err == ENOENT)
		mini->last_exit = err_msg(cmd->cmd_path, ":", \
											" No such file or directory", 127);
	else if (access(cmd->cmd_path, F_OK) != 0)
		mini->last_exit = err_msg(cmd->cmd_path, ":", \
											"  command not found", 127);
	else if (access(cmd->cmd_path, X_OK) != 0 || err == EACCES)
		mini->last_exit = err_msg(cmd->cmd_path, ":", \
											" Permission denied", 126);
	else
		mini->last_exit = err_msg(cmd->cmd_path, ":", strerror(errno), 127);
	safe_exit(mini, mini->last_exit);
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
		ft_execve(mini, cmd, envp);
	}
	safe_exit(mini, mini->last_exit);
}
