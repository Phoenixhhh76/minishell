/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:36:28 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/19 11:44:49 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (!dst && !src)
		return (0);
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}
/*#include <stdio.h>
int main(void)
{

    // Test with NULL pointers
    printf("Real memcpy should crash):\n");
    char *null_src = NULL;
    char *null_dst = NULL;
    // The line will cause a segmentation fault
	memcpy(null_dst, null_src, 3);  
   // printf("Result from real memcpy: %s\n", null_dst);

    // Custom ft_memcpy example:
    printf("\nCustom ft_memcpy test:\n");

    // Test with NULL pointers
    printf("\nft_memcpy with NULL should crash or behave unexpectedly):\n");
    ft_memcpy(null_dst, null_src, 3); 

    return 0;
}
*/
	//if (!dst && !src)
	//	return (0);