/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-23 15:14:07 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-23 15:14:07 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmd *cmd, char ***envp)
{
	int		i;
	char	**env;

	i = 0;
	env = *envp;
	(void)cmd; //maybe needed to check options
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
