/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-22 17:55:17 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-22 17:55:17 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_analyze_argument(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->cmd_args[i])
	{
		if (ft_strchr(cmd->cmd_args[i], '='))
			return (0);
		else
			i++;
	}
	return (-1);
}

int	ft_export(t_cmd *cmd, char ***mini_env)
{
	int		i;
	//char	*key;
	char	**env;

	i = 1;
	env = *mini_env;
	if (ft_analyze_argument(cmd) < 0)
		printf ("export option not found\n");
	return (0);
}