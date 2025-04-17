/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:42:07 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/14 13:15:27 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
/*#include <stdio.h>

char	*ft_strchr(const char *s, int c);

int main() {
	const char *str = "Hello, World!";
	char ch = '\0';

	char *result = ft_strchr(str, ch);
	if (result != NULL)
		printf("'%c' found at position: %ld\n", ch, result - str);
	//%ld vs %td
	else
		printf("'%c' not found\n", ch);
		printf("address ='%p'\n", &result);
		printf("'%c'\n", *result);

	return 0;
}
*/
