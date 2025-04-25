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

bool	is_authorized_character(char c)
{
	if (c == '.' || c == '-' || c == '@' || c == ' ')
		return (false);
	if (ft_isalnum(c))
		return (true);
	return (false);
}

bool	is_only_digits(char *str)
{
	int	i;
	int	cpt;
	int	len;

	i = 0;
	cpt = 0;
	len = ft_strlen(str);
	printf("len= %i\n", len);
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			cpt++;
		i++;
	}
	if (cpt == len)
		return (true);
	return (false);
}

bool	is_valid_var_name(char *var)
{
	int	i;

	i = 1;
	if (!var || !var[0])
		return (false);
	if (var[0] == '=')
	{
		printf("begins with equal sign\n");
		return (false);
	}
	if (!ft_strchr(var, '='))
	{
		printf("no equal sign\n");
		return (false);
	}
	if (is_only_digits(var))
	{
		printf("only digits\n");
		return (false);
	}
	while (var[i])
	{
		if (!is_authorized_character(var[i]))
		{
			printf("unauthorized character\n");
			return (false);
		}
	}
	return (true);
}

// int	find_equal_sign(char *var)
// {
// 	if (ft_strchr(var, '='))
// 			return (0);
// 	return (-1);
// }


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
		if (is_valid_var_name(cmd->cmd_args[i]))
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
				add_var_to_env(env, cmd->cmd_args[i], mini_env);
			free_double_tab(var);
			i++;
		}
		else
		{
			printf("NAME WRONG\n");
			perror("export"); //no equal sign, bad option
			i++;
		}

	}
	return (0);
}

// //could be simplified, no need incrementation of i
// int	find_equal_sign(t_cmd *cmd)
// {
// 	int	i;

// 	i = 1;
// 	while (cmd->cmd_args[i])
// 	{
// 		if (ft_strchr(cmd->cmd_args[i], '='))
// 			return (i);
// 		else
// 			i++;
// 	}
// 	return (-1);
// }

// int	add_var_to_env(char **env, char *cmd, char ***mini_env)
// {
// 	int		i;
// 	int		index;
// 	char	**new_env;

// 	i = 0;
// 	index = 0;
// 	while (env && env[index])
// 		index++;
// 	new_env = (char **)ft_calloc(index + 2, sizeof(char *));
// 	if (!new_env)
// 		return (-1); //alloc error
// 	while (i < index)
// 	{
// 		new_env[i] = ft_strdup(env[i]);
// 		if (!new_env[i])
// 			return(free_double_tab(new_env), -1);
// 		i++;
// 	}
// 	new_env[i++] = ft_strdup(cmd);
// 	new_env[i] = NULL;
// 	free_double_tab(*mini_env);
// 	*mini_env = new_env;
// 	return (0);
// }

// int	ft_export(t_cmd *cmd, char ***mini_env)
// {
// 	int		env_index;
// 	char	**var;
// 	char	**env;

// 	var = NULL;
// 	env = *mini_env;
// 	if (find_equal_sign(cmd) < 0)
// 		return(-1); //no equal sign, bad option
// 	var = ft_split(cmd->cmd_args[1], '=');
// 	if (!var)
// 		return (-1); //split error
// 	env_index = does_var_exist(env, var[0]);
// 	if (env_index >= 0)//var exists
// 	{
// 		free(env[env_index]);
// 		env[env_index] = ft_strdup(cmd->cmd_args[1]);
// 		if (!env[env_index])
// 			return (-1); //dup error
// 	}
// 	else
// 	{
// 		add_var_to_env(env, cmd->cmd_args[1], mini_env);
// 	}
// 	free_double_tab(var);
// 	return (0);
// }