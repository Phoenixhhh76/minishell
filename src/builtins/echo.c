/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:00:20 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/28 10:48:30 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

int	is_valid_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (0);
	if (str[0] != '-' || str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_write_echo(char **args, int i)
{
	int	first;

	first = 1;
	while (args[i])
	{
		if (!first)
			if (write(STDOUT_FILENO, " ", 1) < 0)
				return (perror("echo: write error"), 1);
		if (write(STDOUT_FILENO, args[i],
				strlen(args[i])) < 0)
			return (perror("echo: write error"), 1);
		first = 0;
		i++;
	}
	return (0);
}

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	while (cmd->cmd_args[i] && is_valid_n_option(cmd->cmd_args[i]))
	{
		option = 1;
		i++;
	}
	if (ft_write_echo(cmd->cmd_args, i))
		return (1);
	if (!option)
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (perror("echo: write error"), 1);
	return (0);
}
