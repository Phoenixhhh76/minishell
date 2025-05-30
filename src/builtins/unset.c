/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:29:22 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/20 16:56:54 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_len(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

int	rm_var_from_env(char **env, char ***mini_env, int be_removed)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	new_env = (char **)ft_calloc(env_len(env) + 1, sizeof(char *));
	if (!new_env)
		return (new_env = NULL, -2);
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

int	unset_err_msg(char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	ft_unset(t_cmd *cmd, t_mini *mini)
{
	int		i;
	int		env_index;
	int		status;

	i = 1;
	status = 0;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
		return (0);
	while (cmd->cmd_args[i])
	{
		if (!is_valid_var_name(cmd->cmd_args[i]) \
		|| ft_strchr(cmd->cmd_args[i], '='))
			status = unset_err_msg(cmd->cmd_args[i]);
		else
		{
			env_index = does_var_exist(mini->env, cmd->cmd_args[i]);
			if (env_index >= 0)
				rm_var_from_env(mini->env, &mini->env, env_index);
			if (in_exp_list(mini->exp_list, cmd->cmd_args[i]))
				remove_from_exp_list(&mini->exp_list, cmd->cmd_args[i]);
		}
		i++;
	}
	return (status);
}
