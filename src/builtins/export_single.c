/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_single.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:21:50 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/29 13:33:27 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_export(char *arg, char ***mini_env, t_mini *mini)
{
	char	**env;

	env = *mini_env;
	if (!arg || !arg[0] || arg[0] == '=' || !is_valid_var_name(arg))
		return (export_err_msg(arg, 1));
	if (!ft_strchr(arg, '='))
	{
		if (!is_valid_var_name(arg))
			return (export_err_msg(arg, 1));
		should_store_unassigned_var(arg, env, mini);
		return (0);
	}
	printf("[DEBUG export] checking arg: [%s]\n", arg);
	return (handle_var_with_equal(arg, env, mini_env, mini));
}

void	should_store_unassigned_var(char *arg, char **env, t_mini *mini)
{
	if (does_var_exist(env, arg) < 0 && !in_exp_list(mini->exp_list, arg))
		add_to_exp_list(&mini->exp_list, arg);
}

int	handle_var_with_equal(char *arg,
					char **env, char ***mini_env, t_mini *mini)
{
	char	**var;
	char	*joined;
	int		env_index;

	var = split_export_arg(arg);
	if (!var || !var[0] || var[0][0] == '\0')
		return (free_double_tab(var), -1);
	if (in_exp_list(mini->exp_list, var[0]))
		remove_from_exp_list(&mini->exp_list, var[0]);
	joined = make_joined_assignment(var);
	if (!joined)
		return (free_double_tab(var), -1);
	env_index = does_var_exist(env, var[0]);
	if (handle_var_assignment(env, joined, mini_env, env_index) < 0)
		return (free_double_tab(var), free(joined), -1);
	free_double_tab(var);
	free(joined);
	return (0);
}
