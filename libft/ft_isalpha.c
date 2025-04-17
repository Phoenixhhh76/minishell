/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:32:23 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/13 11:33:55 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}
/*
#include <stdio.h>
int	main(void)
{
	int	i;
	i = 0;
	while (i < 128)
	{
		printf("%d, %c, %d\n", i, i, ft_isalpha(i));
		i ++;
	}
	return (0);
}
*/
