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

// int	ft_search_for_path()
// {

// }

int	ft_cd(t_cmd *cmd, char ***env)
{
	char	*path;

	(void)env;
	path = NULL;
	path = cmd->cmd_args[1];
	if (!path)
		return (-1);
	// if ()
	return (0);
}
