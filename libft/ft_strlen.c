/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:25:22 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/12 12:30:09 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
/*SAFER ONE
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}*/
/*
#include <stdio.h>
#include <string.h> 
int main(void)
{
    const char *str = NULL;
    //size_t len = strlen(str);  // This will likely cause a segmentation fault
	size_t len = ft_strlen(str); 
    printf("Length of the string: %zu\n", len);

    return 0;
}
*/
