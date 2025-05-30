/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:46:24 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/20 12:35:40 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit_errors(t_mini *mini, t_cmd *cmd)
{
	if (cmd->cmd_args[1] && !ft_isnumeric(cmd->cmd_args[1]))
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		if (mini->cpy_in_fd != -1)
			close(mini->cpy_in_fd);
		if (mini->cpy_out_fd != -1)
			close(mini->cpy_out_fd);
		safe_exit(mini, 2);
	}
	if (args_count(cmd->cmd_args) > 2)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	get_exit_code(t_mini *mini, t_cmd *cmd)
{
	int	code;

	code = 0;
	if (ft_isnumeric(cmd->cmd_args[1]))
	{
		if (!ft_is_long_limits(cmd->cmd_args[1]))
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			ft_putstr_fd("minishell: exit: numeric arguments required\n", 2);
			if (mini->cpy_in_fd != -1)
				close(mini->cpy_in_fd);
			if (mini->cpy_out_fd != -1)
				close(mini->cpy_out_fd);
			safe_exit(mini, 2);
		}
		code = ft_atoll(cmd->cmd_args[1]);
		if (code < 0)
			code = 256 + code;
	}
	return (code);
}

int	ft_exit(t_mini *mini, t_cmd *cmd)
{
	int	code;

	if (ft_exit_errors(mini, cmd))
		return (1);
	code = get_exit_code(mini, cmd);
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	if (mini->cpy_in_fd != -1)
		close(mini->cpy_in_fd);
	if (mini->cpy_out_fd != -1)
		close(mini->cpy_out_fd);
	safe_exit(mini, (unsigned char)code);
	return (0);
}
