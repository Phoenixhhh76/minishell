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
			return (free_double_tab(new_env), -1);
		i++;
	}
	new_env[i++] = ft_strdup(cmd);
	new_env[i] = NULL;
	free_double_tab(*mini_env);
	*mini_env = new_env;
	return (0);
}

int	begins_with_digits(char *str)
{
	int	i;

	i = 0;
	while (ft_isdigit(str[i]))
			i++;
	return (i);
}

bool	is_authorized_character(char c)
{
	if (ft_isalnum(c))
		return (true);
	if (c == '_')
		return (true);
	else
		return (false);
}

bool	is_valid_var_name(char *var)
{
	int	i;

	i = 0;
	if (!var || !var[0] || var[0] == '=')
		return (false);
	if (begins_with_digits(var))
		return (false);
	if (!ft_strchr(var, '='))
		return (false);
	while (var[i] && var[i] != '=')
	{
		if (is_authorized_character(var[i]) == false)
			return (false);
		i++;
	}
	return (true);
}

int	ft_export(t_cmd *cmd, char ***mini_env)
{
	int		i;
	int		env_index;
	char	**var;
	char	**env;

	i = 1;
	var = NULL;
	env = *mini_env;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
		return (0);
	while (cmd->cmd_args[i])
	{
		if (is_valid_var_name(cmd->cmd_args[i]) == true)
		{
			var = ft_split(cmd->cmd_args[i], '=');
			if (!var)
				return (-1); //split error
			env_index = does_var_exist(env, var[0]);
			if (env_index >= 0)//var exists
			{
				free(env[env_index]);
				env[env_index] = ft_strdup(cmd->cmd_args[i]);
				if (!env[env_index])
					return (-1); //dup error
			}
			else
			{
				add_var_to_env(env, cmd->cmd_args[i], mini_env);
				env = *mini_env;
			}
			free_double_tab(var);
			i++;
		}
		else
		{
			perror("export"); //no equal sign, bad option
			i++;
		}
	}
	return (0);
}
