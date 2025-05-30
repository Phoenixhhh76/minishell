/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:00:34 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/20 12:31:49 by hho-troc         ###   ########.fr       */
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
	while (var[i] && var[i] != '=')
	{
		if (is_authorized_character(var[i]) == false)
			return (false);
		i++;
	}
	return (true);
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
		return (-1);
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

bool	in_exp_list(char **exp_list, const char *key)
{
	int	i;

	i = 0;
	while (exp_list && exp_list[i])
	{
		if (ft_strcmp(exp_list[i], key) == 0)
			return (true);
		i++;
	}
	return (false);
}
