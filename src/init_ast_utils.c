/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ast_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndabbous <ndabbous@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-04-17 12:23:08 by ndabbous          #+#    #+#             */
/*   Updated: 2025-04-17 12:23:08 by ndabbous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*create_node(char *splited)
{
	t_ast	*node;
	int		content;

	content = ft_atol(splited);
	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (0);
	node->content = content;
	node->next = NULL;
	return (node);
}

void	ft_astadd_back(t_ast **stack_a, t_ast *new)
{
	t_ast	*tmp;

	if (!(*stack_a))
	{
		*stack_a = new;
		return ;
	}
	tmp = *stack_a;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_ast	*ft_astlast(t_ast *stack)
{
	while (stack != NULL)
	{
		if (!(stack->next))
			return (stack);
		stack = stack->next;
	}
	return (NULL);
}

t_ast	*ft_penultimate(t_ast *stack)
{
	while (stack != NULL)
	{
		if (!(stack->next->next))
			return (stack);
		stack = stack->next;
	}
	return (NULL);
}

int	ft_astsize(t_ast **stack)
{
	int		i;
	t_ast	*tmp;

	i = 0;
	tmp = *stack;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}
