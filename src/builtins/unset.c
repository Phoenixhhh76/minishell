/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-23 15:29:22 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-23 15:29:22 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	rm_var_from_env(char **env, char ***mini_env, int be_removed)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while (env && env[i])
		i++;
	new_env = (char **)ft_calloc(i + 1, sizeof(char *));
	if (!new_env)
		return (-1); //alloc error
	i = 0;
	while (env[i])
	{
		if (i != be_removed)
		{
			new_env[j] = ft_strdup(env[i]);
			if (!new_env[j])
				return (free_double_tab(new_env), -1);
			j++;
		}
		i++;
	}
	new_env[j] = NULL;
	free_double_tab(*mini_env);
	*mini_env = new_env;
	return (0);
}

int	ft_unset(t_cmd *cmd, char ***mini_env)
{
	int		i;
	int		env_index;
	char	**env;

	i = 1;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
		return (0);
	while (cmd->cmd_args[i])
	{
		env = *mini_env;
		env_index = does_var_exist(env, cmd->cmd_args[i]);
		if (env_index >= 0)
			rm_var_from_env(env, mini_env, env_index);
		i++;
	}
	return (0);
}
