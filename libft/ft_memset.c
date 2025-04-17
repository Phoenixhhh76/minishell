/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:36:36 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/12 11:07:46 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*
void	*ft_memset(void *s, int c, size_t len)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (i < len)
	{
		*(unsigned char *)(s + i) = (unsigned char) c;
		i++;
	}
	return (s);
}
*/