/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 14:44:38 by hho-troc          #+#    #+#             */
/*   Updated: 2025/05/29 13:58:57 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_var_assignment(char **env, char *joined, char ***mini_env, int index)
{
	if (!joined)
		return (-1);
	if (index >= 0)
	{
		free(env[index]);
		env[index] = ft_strdup(joined);
		if (!env[index])
			return (-1);
	}
	else
	{
		if (add_var_to_env(env, joined, mini_env) < 0)
			return (-1);
	}
	return (0);
}

void	add_to_exp_list(char ***exp_list, const char *key)
{
	int		len;
	char	**new_list;
	int		i;

	len = 0;
	i = 0;
	while (*exp_list && (*exp_list)[len])
		len++;
	new_list = ft_calloc(len + 2, sizeof(char *));
	while (i < len)
	{
		new_list[i] = ft_strdup((*exp_list)[i]);
		i++;
	}
	new_list[i] = ft_strdup(key);
	new_list[i + 1] = NULL;
	free_double_tab(*exp_list);
	*exp_list = new_list;
}

void	remove_from_exp_list(char ***exp_list, const char *key)
{
	int		i;
	int		j;
	int		len;
	char	**new_list;

	i = 0;
	j = 0;
	len = 0;
	while (*exp_list && (*exp_list)[len])
		len++;
	new_list = ft_calloc(len, sizeof(char *));
	if (!new_list)
		return ;
	while ((*exp_list)[i])
	{
		if (ft_strcmp((*exp_list)[i], key) != 0)
			new_list[j++] = ft_strdup((*exp_list)[i]);
		i++;
	}
	new_list[j] = NULL;
	free_double_tab(*exp_list);
	*exp_list = new_list;
}

int	ft_export(t_cmd *cmd, char ***mini_env, t_mini *mini)
{
	int		i;
	int		status;

	i = 1;
	status = 0;
	if (!cmd || !cmd->cmd_args || !cmd->cmd_args[1])
	{
		print_export_env(*mini_env, mini->exp_list);
		return (0);
	}
	while (cmd->cmd_args[i])
	{
		if (!cmd->cmd_args[i] || cmd->cmd_args[i][0] == '\0')
		{
		//	printf("[DEBUG] export invalid arg: [%s]\n", cmd->cmd_args[i]);
			export_err_msg(cmd->cmd_args[i], 1);
			status = 1;
			i++;
			continue ;
		}
		else if (handle_single_export(cmd->cmd_args[i], mini_env, mini) != 0)
			status = 1;
		//printf("[DEBUG export loop] arg: [%s]\n", cmd->cmd_args[i]);
		i++;
	}
	mini->last_exit = status;
	return (status);
}

