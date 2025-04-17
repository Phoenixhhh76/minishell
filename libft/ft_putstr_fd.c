/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:41:59 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/19 11:52:57 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char const *s, int fd)
{
	int	i;

	i = 0;
	if (fd < 0)
		return ;
	while (s[i])
	{	
		write(fd, &s[i], 1);
		i++;
	}
}
/*#include <stdio.h>
void	ft_putchar_fd(char c, int fd)
{
	write (fd, &c, 1);
}
int main(void)
{
    ft_putstr_fd("\nend!", -1);  // Invalid file descriptor
    return 0;
}
*/