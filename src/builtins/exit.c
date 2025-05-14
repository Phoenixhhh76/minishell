/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-06 13:46:24 by ndabbous          #+#    #+#             */
/*   Updated: 2025-05-06 13:46:24 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	ft_exit(t_cmd *cmd)
// {
// 	int	code;

// 	printf("exit\n");
// 	if (cmd->cmd_args[1])
// 	{
// 		if (!ft_isdigit(cmd->cmd_args[1]))
// 		{
// 			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
// 			//safe_cleanup(mini, NULL); 
// 			exit(255);
// 		}
// 		if (cmd->cmd_args[2])
// 		{
// 			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
// 			return (1); 
// 		}
// 		code = ft_atoi(cmd->cmd_args[1]);
// 		//safe_cleanup(mini, NULL);
// 		exit((unsigned char)code);
// 	}
// 	//safe_cleanup(mini, NULL);
// 	//exit(mini->last_exit);
// 	exit(0);
// }


//long max lomg min
//error code, use %256