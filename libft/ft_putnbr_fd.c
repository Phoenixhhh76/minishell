/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:41:56 by hho-troc          #+#    #+#             */
/*   Updated: 2024/11/14 13:45:25 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	num;

	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		num = (unsigned int)(-n);
	}
	else
		num = (unsigned int)n;
	if (num >= 10)
		ft_putnbr_fd(num / 10, fd);
	ft_putchar_fd((char)(num % 10 + '0'), fd);
}
/*
#include <unistd.h>

// Mock implementation of ft_putchar_fd for testing purposes
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// Prototype of the function to be tested
void	ft_putnbr_fd(int n, int fd);

int main(void)
{
	// Test positive numbers
	ft_putnbr_fd(42, 1);
	write(1, "\n", 1);

	ft_putnbr_fd(2147483647, 1);
	write(1, "\n", 1);

	// Test zero
	ft_putnbr_fd(0, 1);
	write(1, "\n", 1);

	// Test negative numbers
	ft_putnbr_fd(-42, 1);
	write(1, "\n", 1);

	ft_putnbr_fd(-2147483648, 1);  // Testing the edge case for INT_MIN
	write(1, "\n", 1);

	return 0;
}
*/
