/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:56:53 by ndabbous          #+#    #+#             */
/*   Updated: 2025/05/20 11:55:06 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_mini	*static_struct(t_mini *mini)
{
	static t_mini	*tmp = NULL;

	if (mini)
		tmp = mini;
	return (tmp);
}

char	**copy_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = (char **)ft_calloc(i + 1, sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	init_mini(t_mini *mini, char **av, char **env)
{
	mini->token = NULL;
	mini->ast = NULL;
	mini->av = av;
	mini->stop_hd = 0;
	mini->cpy_in_fd = -1;
	mini->cpy_out_fd = -1;
	mini->last_exit = 0;
	mini->exp_list = NULL;
	mini->env = copy_env(env);
	if (!mini->env)
		safe_exit(mini, 17);
}
