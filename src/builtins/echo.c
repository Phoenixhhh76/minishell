/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-23 11:44:19 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-23 11:44:19 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	option;

	i = 1;
	option = 0;
	print_cmd(cmd);
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
}
