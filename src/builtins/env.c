/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:14:07 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/28 10:53:52 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_cmd *cmd, char ***envp)
{
	int		i;
	char	**env;

	i = 0;
	env = *envp;
	(void)cmd;
	while (env[i])
	{
		if (write(STDOUT_FILENO, env[i], strlen(env[i])) < 0 || \
			write(STDOUT_FILENO, "\n", 1) < 0)
		{
			perror("env: write error");
			return (125);
		}
		i++;
	}
	return (0);
}
