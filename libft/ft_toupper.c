/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:43:52 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/14 09:43:20 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
/*#include <stdio.h>
int main(void)
{
    char upper = 'A';
    char non_alpha = '1';

    printf("'%c' is '%c'\n", upper, ft_tolower(upper));  
    printf("'%c' is '%c'\n", non_alpha, ft_tolower(non_alpha));  
    return (0);
}*/
