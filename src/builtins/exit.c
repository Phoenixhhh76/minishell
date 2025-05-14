/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-06 13:46:24 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-06 13:46:24 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_isnumeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (false);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(t_mini *mini, t_cmd *cmd)
{
	int	code;

	(void)mini;
	code = 0;
	if (cmd->cmd_args[1] && !ft_isnumeric(cmd->cmd_args[1]))
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		safe_exit(mini, 2);
	}
	if (cmd->cmd_args[2])
	{
		printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		//ft_update_last_exit_value(mini, 1);
		return (1);
	}
	if (ft_isnumeric(cmd->cmd_args[1]))
	{
		code = ft_atoi(cmd->cmd_args[1]);
		if (code < 0)
			code = 256 + code;
	}
	printf("exit\n");
	safe_exit(mini, (unsigned char)code);
	return (0);
	//exit(mini->last_exit);
}

// long max lomg min
// error code, use %256