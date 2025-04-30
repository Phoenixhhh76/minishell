/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:19:28 by hho-troc          #+#    #+#             */
/*   Updated: 2025/04/23 17:11:14 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_error_pipe(int error_status, t_cmd *cmd)
{
	if (cmd)
	{
		close_fds(cmd);
		if (cmd->pipe != NULL)
			free(cmd->pipe);
		if (cmd->pids != NULL)
			free(cmd->pids);
		//if (cmd->cmd_opts != NULL || cmd->cmd_path != NULL)//we don't use cmd->opts
		//	free_strs(cmd->cmd_path, cmd->cmd_opts);
	}
	if (cmd->heredoc_nb == 1)
		unlink(".heredoc.tmp");
	exit(error_status);
}

int	err_msg(char *str1, char *str2, char *str3, int erno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putendl_fd(str3, 2);
	return (erno);
}

void exit_error(const char *msg)
{
	perror(msg);
	//need to care about memory leak
	exit(1);
}
