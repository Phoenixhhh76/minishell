/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-22 14:38:35 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-22 14:38:35 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(t_cmd *cmd)
{
	char	*path;

	path = cmd->cmd_args[1];
	if (!path)
		path = getenv("HOME");
	if (chdir(path) < 0)
	{
		perror("cd");
		return (-1);
	}
	return (0);
}
