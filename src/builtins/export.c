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

int	does_var_exist(char **env, const char *var)
{
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
//could be simplified, no need incrementation of i
int	find_equal_sign(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->cmd_args[i])
	{
		if (ft_strchr(cmd->cmd_args[i], '='))
			return (i);
		else
			i++;
	}
	return (-1);
}

void	free_double_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

int	add_var_to_env(char **env, char *cmd, char ***mini_env)
{
	int		i;
	int		index;
	char	**new_env;

	i = 0;
	index = 0;
	while (env && env[index])
		index++;
	new_env = (char **)ft_calloc(index + 2, sizeof(char *));
	if (!new_env)
		return (-1); //alloc error
	while (i < index)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return(free_double_tab(new_env), -1);
		i++;
	}
	new_env[i++] = ft_strdup(cmd);
	new_env[i] = NULL;
	free_double_tab(*mini_env);
	*mini_env = new_env;
	return (0);
}

int	ft_export(t_cmd *cmd, char ***mini_env)
{
	int		env_index;
	char	**var;
	char	**env;

	var = NULL;
	env = *mini_env;
	if (find_equal_sign(cmd) < 0)
		return(-1); //no equal sign, bad option
	var = ft_split(cmd->cmd_args[1], '=');
	if (!var)
		return (-1); //split error
	env_index = does_var_exist(env, var[0]);
	if (env_index >= 0)//var exists
	{
		printf(">>>>>>FOUND VAR\n");
		free(env[env_index]);
		env[env_index] = ft_strdup(cmd->cmd_args[1]);
		if (!env[env_index])
			return (-1); //dup error
	}
	else
	{
		printf(">>>>>>ADD VAR\n");
		add_var_to_env(env, cmd->cmd_args[1], mini_env);
	}
	free_double_tab(var);
	return (0);
}
