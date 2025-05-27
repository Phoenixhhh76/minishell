/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:00:20 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/26 12:01:31 by hho-troc         ###   ########.fr       */
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
