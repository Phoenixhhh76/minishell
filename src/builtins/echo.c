/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:00:20 by ndabbous          #+#    #+#             */
/*   Updated: 2025/04/30 18:19:29 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	if (!ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		printf("%s", cmd->cmd_args[i]);
		if (cmd->cmd_args[i + 1])
			printf(" ");
		i++;
	}
	if (option == 0)
		printf("\n");
} */

//test for ''$USER''//
/*
void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;
	int	first_printed;

	i = 1;
	option = 0;
	first_printed = 0;
	//print_cmd(cmd);
	if (!ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		if (first_printed)
			printf(" ");
		printf("%s", cmd->cmd_args[i]);
		first_printed = 1;
		i++;
	}
	if (option == 0)
		printf("\n");
} */
//skip the first empty cmd as space
/* void	ft_echo(t_cmd *cmd)
{
	int	i = 1;
	int	option = 0;
	int	first = 1;

	if (cmd->cmd_args[i] && !ft_strcmp(cmd->cmd_args[i], "-n"))
	{
		option = 1;
		i++;
	}
	while (cmd->cmd_args[i])
	{
		if (cmd->cmd_args[i][0] != '\0')
	// change here because we need to print empty string "" as space
		{
			if (!first)
				printf(" ");
			printf("%s", cmd->cmd_args[i]);
			first = 0;
		}
		i++;
	}
	if (!option)
		printf("\n");
} */
//skip the first empty cmd as space, and take empty cmd as space//
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
			printf(" ");
		printf("%s", cmd->cmd_args[i]);
		first = 0;
		i++;
	}
	if (!option)
		printf("\n");
	return (0);
}
