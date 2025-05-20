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

bool	ft_is_long_limits(const char *nptr)
{
	int			i;
	int			sign;
	int			digit;
	long long	nb;

	i = 0;
	nb = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - 48;
		if (sign == 1 && (nb > (LLONG_MAX - digit) / 10))
			return (false);
		if (sign == -1 && (-nb < (LLONG_MIN + digit) / 10))
			return (false);
		nb = nb * 10 + (nptr[i] - 48);
		i++;
	}
	return (true);
}

long long	ft_atoll(const char *nptr)
{
	int			i;
	int			sign;
	long long	nb;

	i = 0;
	nb = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i] - 48);
		i++;
	}
	return (nb * sign);
}

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

int	args_count(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	ft_exit(t_mini *mini, t_cmd *cmd)
{
	int	code;

	code = 0;
	//print_cmd(cmd);
	if (cmd->cmd_args[1] && !ft_isnumeric(cmd->cmd_args[1]))
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		safe_exit(mini, 2);
	}
	if (args_count(cmd->cmd_args) > 2)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		//ft_update_last_exit_value(mini, 1);
		return (1);
	}
	if (ft_isnumeric(cmd->cmd_args[1]))
	{
		if (!ft_is_long_limits(cmd->cmd_args[1]))
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
			ft_putstr_fd("minishell: exit: numeric arguments required\n", 2);
			safe_exit(mini, 2);
		}
		code = ft_atoll(cmd->cmd_args[1]);
		if (code < 0)
			code = 256 + code;
	}
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	safe_exit(mini, (unsigned char)code);
	return (0);
}
