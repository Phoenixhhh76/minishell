/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 16:19:28 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/16 16:01:22 by hho-troc         ###   ########.fr       */
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
	}
	if (cmd->heredoc_nb == 1)
		unlink(".heredoc.tmp");//why ? we dont use .tmp with heredoc ?
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
	exit(1);//Nina
}

int	syntax_err_msg(char *str1, char *str2, int erno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("syntax error ", 2);
	ft_putstr_fd(str1, 2);
	ft_putendl_fd(str2, 2);
	return (erno);
}

int	export_err_msg(char *arg, int erno)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (erno);
}