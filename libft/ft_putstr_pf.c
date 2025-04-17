/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_pf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hho-troc <hho-troc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:54:43 by hho-troc          #+#    #+#             */
/*   Updated: 2025/02/06 11:18:30 by hho-troc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_pf(char *str, size_t *counter)
{
	if (!str)
	{
		*counter += write(1, "(null)", 6);
		return ;
	}
	while (*str)
		ft_putchar_pf(*str++, counter);
}
