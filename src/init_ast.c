/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-15 14:48:52 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-15 14:48:52 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_ast(t_mini *mini, t_ast *ast)
{
	int	i;
	t_ast	

	i = 0;
	while (i < size_token(mini->token))
	{

	}
}

//if pipe = at least 1 ast_node and 2 cmd 
//if no pipe = 1 ast_node, create 