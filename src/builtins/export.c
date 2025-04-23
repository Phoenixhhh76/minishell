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

// bool	is_valid_var_name(char *var)
// {
// 	int	i;
// 	int	flag_alpha;

// 	i = 1;
// 	flag_alpha = 0;
// 	if (!ft_strchr(var, '='))
// 		return (false);
// 	if (!var || !var[0])
// 		return (false);
// 	if (ft_isdigit(var[0]))
// 	{
// 		while (ft_isdigit(var[i]))
// 			i++;
// 	}
// 	while (var[i])
// 	{
// 		if (ft_isalpha(var[i]) || var[i] == '_')
// 		{
// 			flag_alpha++;
// 			i++;
// 		}
// 		if (var[i] == '_')
// 			i++;
// 		if (ft_isdigit(var[i]))
// 			return (false);
// 		i++;
// 	}
// 	if (flag_alpha != 0)
// 		return (true);
// 	return (false);
// }

// //could be simplified, no need incrementation of i
// // int	find_equal_sign(char *var)
// // {
// // 	if (ft_strchr(var, '='))
// // 			return (0);
// // 	return (-1);
// // }


// int	ft_export(t_cmd *cmd, char ***mini_env)
// {
// 	int		i;
// 	int		env_index;
// 	char	**var;
// 	char	**env;

// 	i = 1;
// 	var = NULL;
// 	env = *mini_env;
// 	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
// 		return (0);
// 	while (cmd->cmd_args[i])
// 	{
// 		if (!is_valid_var_name(cmd->cmd_args[i]))
// 		{
// 			printf("NAME WRONG\n");
// 			return(-1); //no equal sign, bad option
// 		}
// 		var = ft_split(cmd->cmd_args[i], '=');
// 		if (!var)
// 			return (-1); //split error
// 		env_index = does_var_exist(env, var[0]);
// 		if (env_index >= 0)//var exists
// 		{
// 			free(env[env_index]);
// 			env[env_index] = ft_strdup(cmd->cmd_args[i]);
// 			if (!env[env_index])
// 				return (-1); //dup error
// 		}
// 		else
// 			add_var_to_env(env, cmd->cmd_args[i], mini_env);
// 		free_double_tab(var);
// 		i++;
// 	}
// 	return (0);
// }

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
		free(env[env_index]);
		env[env_index] = ft_strdup(cmd->cmd_args[1]);
		if (!env[env_index])
			return (-1); //dup error
	}
	else
	{
		add_var_to_env(env, cmd->cmd_args[1], mini_env);
	}
	free_double_tab(var);
	return (0);
}