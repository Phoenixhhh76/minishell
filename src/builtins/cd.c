/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:38:35 by ndabbous          #+#    #+#             */
/*   Updated: 2025/04/30 17:41:38 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(t_cmd *cmd)
{
	char	*path;

	if (cmd->cmd_args[1] && cmd->cmd_args[2])
	{
		err_msg("cd", ":", " too many arguments", 1);
		return (1);
	}
	path = cmd->cmd_args[1];
	if (!path)
		path = getenv("HOME");
	if (chdir(path) < 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
