/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:43:43 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/13 19:02:26 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	i;
	char	*str;

	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_calloc(1, sizeof(char)));
	if (s_len - start < len)
		len = s_len - start;
	str = ft_calloc(len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	return (str);
}
/*#include <stdio.h>

int main(void)
{
	char *s = "Hello, World!";
	char *result;

	result = ft_substr(s, 7, 5);
	printf("Result 1: %s\n", result); // "World"
	free(result);

	// star >= s_len
	result = ft_substr(s, 20, 5);
	printf("Result 2: %s\n", result); // ""
	free(result);

	// s_len - start < len
	result = ft_substr(s, 7, 20);
	printf("Result 3: %s\n", result); // "World!"
	free(result);

	return 0;
}
*/