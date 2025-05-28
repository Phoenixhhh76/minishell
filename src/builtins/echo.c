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
#include "../../includes/minishell.h"

int	is_valid_n_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
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

// int	ft_echo(t_cmd *cmd)
// {
// 	int	i;
// 	int	option;
// 	int	first;
// 	int	printf_result;

// 	i = 1;
// 	option = 0;
// 	first = 1;
// 	while (cmd->cmd_args[i] && is_valid_n_option(cmd->cmd_args[i]))
// 	{
// 		option = 1;
// 		i++;
// 	}
// 	while (cmd->cmd_args[i])
// 	{
// 		if (!first)
// 			printf(" ");
// 		printf_result=printf("%s", cmd->cmd_args[i]); //echo a > /dev/full becareful
// 		if (printf_result < 0)
// 		{
// 			perror("echo");
// 			return (1);
// 		}
// 		first = 0;
// 		i++;
// 	}
// 	if (!option)
// 		printf("\n");
// 	return (0);
// }

int	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;
	int	first;


	i = 1;
	option = 0;
	first = 1;
	while (cmd->cmd_args[i] && is_valid_n_option(cmd->cmd_args[i]))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		if (!first)
			if (write(STDOUT_FILENO, " ", 1) < 0)
				return (perror("echo: write error"), 1);
		if (write(STDOUT_FILENO, cmd->cmd_args[i],
				strlen(cmd->cmd_args[i])) < 0)
			return (perror("echo: write error"), 1);
		first = 0;
		i++;
	}
	if (!option)
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (perror("echo: write error"), 1);
	return (0);
}
