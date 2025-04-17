/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 08:43:08 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/13 17:31:28 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*new;

	if (size != 0 && count > (SIZE_MAX / size))
		return (NULL);
	new = malloc(count * size);
	if (!new)
		return (NULL);
	ft_bzero(new, count * size);
	return (new);
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void ft_bzero(void *s, size_t n) {
	unsigned char *ptr = s;
	while (n--) {
		*ptr++ = 0;
	}
}

int main() {
	size_t count = 5;
	size_t size = sizeof(int);
	int *array = (int *)ft_calloc(count, size);

	if (!array) {
		printf("Memory allocation failed.\n");
		return 1;
	}

	printf("Allocated memory and initialized to zero:\n");
	for (size_t i = 0; i < count; i++) {
		printf("array[%zu] = %d\n", i, array[i]);
	}

	free(array);
	return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);

void ft_bzero(void *s, size_t n) {
	unsigned char *ptr = s;
	while (n--) {
		*ptr++ = 0;
	}
}

int main() {
	size_t large_count = SIZE_MAX;
	size_t small_size = 2;
	void *result1 = ft_calloc(large_count, small_size);
	if (result1 == NULL) {
		printf("Test 1: Overflow detected, returned NULL as expected.\n");
	} else {
		printf("Test 1: Unexpected result, memory allocation failed.\n");
		free(result1);
	}

	size_t normal_count = 2;
	size_t large_size = SIZE_MAX;
	void *result2 = ft_calloc(normal_count, large_size);
	if (result2 == NULL) {
		printf("Test 2: Overflow detected, returned NULL as expected.\n");
	} else {
		printf("Test 2: Unexpected result, memory allocation failed.\n");
		free(result2);
	}

	size_t count = SIZE_MAX / 2 + 1;
	size_t size = 2;
	void *result3 = ft_calloc(count, size);
	if (result3 == NULL) {
		printf("Test 3: Overflow detected, returned NULL as expected.\n");
	} else {
		printf("Test 3: Unexpected result, memory allocation failed.\n");
		free(result3);
	}

	return 0;
}
*/
